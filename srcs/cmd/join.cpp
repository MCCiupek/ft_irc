#include "headers.hpp"

/*
	Command: JOIN
	Parameters: <channel>{,<channel>} [<key>{,<key>}]

	The JOIN command is used by client to start listening a specific
	channel. Whether or not a client is allowed to join a channel is
	checked only by the server the client is connected to; all other
	servers automatically add the user to the channel when it is received
	from other servers.  The conditions which affect this are as follows:

		1.  the user must be invited if the channel is invite-only;
		2.  the user's nick/username/hostname must not match any
               active bans;

		3.  the correct key (password) must be given if it is set.

	These are discussed in more detail under the MODE command (see
	section 4.2.3 for more details).

	Once a user has joined a channel, they receive notice about all
	commands their server receives which affect the channel.  This
	includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.  The
	JOIN command needs to be broadcast to all servers so that each server
	knows where to find the users who are on the channel.  This allows
	optimal delivery of PRIVMSG/NOTICE messages to the channel.

	If a JOIN is successful, the user is then sent the channel's topic
	(using RPL_TOPIC) and the list of users who are on the channel (using
	RPL_NAMREPLY), which must include the user joining.

	Numeric Replies:

		ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
		ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
		ERR_CHANNELISFULL               ERR_BADCHANMASK
		ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
		RPL_TOPIC
*/

int		create_channel( string channel, string key, User &u, Server &srv ) {

	Channel	* new_channel = new Channel(channel, key, "", &u, "nt");

	srv.addChannel( new_channel );
	u.addChannel( new_channel );
	u.setCurrChan( new_channel );

	send_reply(u, 353, RPL_NAMREPLY(new_channel->getName(), new_channel->getMembersList()));
	send_reply(u, 366, RPL_ENDOFNAMES(new_channel->getName()));
	return 0;
}

int		join_channel( string channel, string key, User &usr, Server &srv ) {

	Channel 		*cnl;

	if ( !usr.isRegistered() )
	{
		send_error( usr, ERR_NOTREGISTERED, "JOIN" );
		return 1;
	}		

	if ( channel[0] == '#' ) {
		cnl = srv.getChannelByName( channel );
		if ( cnl == NULL ) // Create Channel. 
		{
			send_notice(usr, usr, NTC_JOIN(channel));
			return create_channel(channel, key, usr, srv);
		}
	}
	else {
		send_error( usr, ERR_BADCHANMASK, channel );
		return 1;
	}
	if ( cnl->isSecret() ) {
		send_error( usr, ERR_NOSUCHCHANNEL, channel );
		return 1;
	}

	if (usr.getCurrChan())
	{
		// User is registred in channel and it is the current
		if ( (usr.getCurrChan())->getName() == channel )
			return 0;
		// User is registered in channel but is not the current
		else if ( usr.isRegisteredToChan(*cnl) && (usr.getCurrChan()->getName()) != channel ) {
			usr.setCurrChan(cnl);
			return 0;
		}
	}

	if ( cnl->getHasKey() && cnl->getKey() != key ) {
		send_error( usr, ERR_BADCHANNELKEY, channel );
		return 1;
	}
	if ( cnl->isBanned( usr ) ) {
		send_error( usr, ERR_BANNEDFROMCHAN, channel );
		return 1;
	}
	if ( cnl->getNbMembers() == cnl->getLimit() ) {
		send_error( usr, ERR_CHANNELISFULL, channel );
		return 1;
	}
	if ( cnl->isInviteOnly() && !cnl->isInvited(usr) ) {
		send_error( usr, ERR_INVITEONLYCHAN, channel );
		return 1;
	}
	cnl->addMember(&usr);
	usr.addChannel( cnl );
	usr.setCurrChan( cnl );
	vector<User*> usrs = cnl->getMembers();
	for (size_t i = 0; i < usrs.size(); i++)
		send_notice(usr, *usrs[i], NTC_JOIN(channel));
	if (cnl->getHasTopic()) {
		send_reply(usr, 332, RPL_TOPIC(cnl->getName(), cnl->getTopic()));
		send_reply(usr, 333, RPL_TOPICWHOTIME(cnl->getName(), cnl->getTopicWho()->fci(), cnl->getTopicWhen()));
	}
	send_reply(usr, 353, RPL_NAMREPLY(cnl->getName(), cnl->getMembersList()));
	send_reply(usr, 366, RPL_ENDOFNAMES(cnl->getName()));
	return 0;
}

void		join( vector<string> args, User &usr, Server &srv ) {

	vector<string>	chans;

	if ( args.size() < 1 ) {
		send_error( usr, ERR_NEEDMOREPARAMS, "JOIN" );
		return ;
	}
	
	chans = ft_split(args[0], ",");

	vector<string>	keys(chans.size());

	if (args.size() > 1)
		keys = ft_split(args[1], ",");

	for (size_t i = 0; i < chans.size(); i++) {
		if ( i + 1 == MAX_CHAN_PER_USR ) {
			send_error( usr, ERR_TOOMANYCHANNELS, "JOIN" );
			return ;
		}
		join_channel(chans[i], keys[i], usr, srv);	
	}
}