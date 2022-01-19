#include "headers.hpp"

/*
	Command: PART
	Parameters: <channel> *( "," <channel> ) [ <Part Message> ]

	The PART command causes the user sending the message to be removed
	from the list of active members for all given channels listed in the
	parameter string.  If a "Part Message" is given, this will be sent
	instead of the default message, the nickname.  This request is always
	granted by the server.

	Servers MUST be able to parse arguments in the form of a list of
	target, but SHOULD NOT use lists when sending PART messages to
	clients.

	Numeric Replies:

		ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
		ERR_NOTONCHANNEL

   Examples:

   PART #twilight_zone             ; Command to leave channel
                                   "#twilight_zone"

   PART #oz-ops,&group5            ; Command to leave both channels
                                   "&group5" and "#oz-ops".

   :WiZ!jto@tolsun.oulu.fi PART #playzone :I lost
                                   ; User WiZ leaving channel
                                   "#playzone" with the message "I
                                   lost".
*/

void		part( vector<string> args, User &usr, Server &srv ) {

	vector<string>	chans;
	string			part_msg;// = usr.getNick() + " left channel";
	Channel *		cnl;
	
	if ( args.size() < 1 ) {
		send_error( usr, ERR_NEEDMOREPARAMS, "PART" );
		return ;
	}

	chans = ft_split(args[0], ",");

	if ( args.size() > 1 )
		part_msg = ft_join(args, " ", 1);

	for (size_t i = 0; i < chans.size(); i++) {

		cnl = srv.getChannelByName( chans[i] );

		// if channel doesnt exists
		if ( cnl == NULL ) {
			send_error( usr, ERR_NOSUCHCHANNEL, chans[i] );
			continue ;
		}

		// if user is not on channel
		if ( !cnl->isOnChann(usr) ) {
			send_error( usr, ERR_NOTONCHANNEL, chans[i] );
			continue ;
		}

		//send_to_all_in_chan(cnl, part_msg, usr);

		vector<User*> usrs = cnl->getMembers();
		for (size_t j = 0; j < usrs.size(); j++) {
			if ( args.size() == 1 )
				send_notice(usr, *usrs[j], NTC_PART(cnl->getName()));
			else
				send_notice(usr, *usrs[j], NTC_PART_MSG(cnl->getName(), &part_msg[1]));
		}
	
		usr.deleteChannel(cnl);

		// TODO: PUT THE FOLLOWING CODE IN USER::DELETECHANNEL() >>>>>>>>>>>>>>>>>
		// if channel is user's current channel
		if ( usr.getCurrChan()->getName() == cnl->getName() ) {
			if ( usr.getChannels().size() > 0 )
				usr.setCurrChan(usr.getChannels().back());
			else
				usr.setCurrChan(nullptr);
		}
		// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// Delete chan if usr leaving is the last usr in chan
		if (cnl->getNbMembers() == 0)
			srv.deleteChannel(cnl);
	}
}