#include "headers.hpp"

/*
	Command: INVITE
	Parameters: <nickname> <channel>

	The INVITE message is used to invite users to a channel.  The
	parameter <nickname> is the nickname of the person to be invited to
	the target channel <channel>.  There is no requirement that the
	channel the target user is being invited to must exist or be a valid
	channel.  To invite a user to a channel which is invite only (MODE
	+i), the client sending the invite must be recognised as being a
	channel operator on the given channel.

	Numeric Replies:

			ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
			ERR_NOTONCHANNEL                ERR_USERONCHANNEL
			ERR_CHANOPRIVSNEEDED
			RPL_INVITING                    RPL_AWAY

	Examples:

	:Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel
									#Dust

	INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
									#Twilight_zone
*/

void		invite( vector<string> args, User &usr, Server &srv ) {

	Channel *		cnl;
	User	*		guest;
	
	if ( args.size() < 2 ) {
		send_error( usr, ERR_NEEDMOREPARAMS, "INVITE" );
		return ;
	}

	cnl = srv.getChannelByName( args[1] );

	// if channel doesnt exists
	if ( cnl == NULL ) {
		return ;
	}

	// if user is not on channel
	if ( !cnl->isOnChann(usr) )
		return send_error( usr, ERR_NOTONCHANNEL, args[1] );

	// if user is not channel operator ans channel is invite-only
	if ( !cnl->isOper(usr) && cnl->isInviteOnly() )
		return send_error( usr, ERR_CHANOPRIVSNEEDED, args[1] );

	guest = srv.getUserByNick(args[0]);

	// if guest's nick is not a valid nick
	if ( !guest )
		return send_error( usr, ERR_NOSUCHNICK, guest->getNick() );

	// if guest is already on channel
	if ( cnl->isOnChann(*guest) )
		return send_error( usr, ERR_USERONCHANNEL, args[0] + " " + args[1] );

	//vector<User*> usrs = cnl->getMembers();
	//for (size_t i = 0; i < usrs.size(); i++) {
	//	send_notice(usr, *usrs[i], NTC_INVITE(cnl->getName(), guest->getNick()));
	//}

	send_notice(usr, *guest, NTC_INVITE(cnl->getName(), guest->getNick()));

	cnl->invite(guest);

	send_reply(usr, 341, RPL_INVITING(guest->getNick(), cnl->getName()));
}