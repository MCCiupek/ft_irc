/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanetti <fmanetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 23:49:15 by fmanetti          #+#    #+#             */
/*   Updated: 2021/12/21 17:06:38 by fmanetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"

/*
	Command: MODE

	The MODE command is a dual-purpose command in IRC.  It allows both
	usernames and channels to have their mode changed.  The rationale for
	this choice is that one day nicknames will be obsolete and the
	equivalent property will be the channel.

	When parsing MODE messages, it is recommended that the entire message
	be parsed first and then the changes which resulted then passed on.

	1. Channel modes

		Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
			[<ban mask>]

		The MODE command is provided so that channel operators may change the
		characteristics of `their' channel.  It is also required that servers
		be able to change channel modes so that channel operators may be
		created.

		The various modes available for channels are as follows:

			o - give/take channel operator privileges;
			p - private channel flag;
			s - secret channel flag;
			i - invite-only channel flag;
			t - topic settable by channel operator only flag;
			n - no messages to channel from clients on the outside;
			m - moderated channel;
			l - set the user limit to channel;
			b - set a ban mask to keep users out;
			v - give/take the ability to speak on a moderated channel;
			k - set a channel key (password).

		When using the 'o' and 'b' options, a restriction on a total of three
		per mode command has been imposed.  That is, any combination of 'o'
		and ...

		Numeric Replies:

           ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
           ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_KEYSET
           RPL_BANLIST                     RPL_ENDOFBANLIST
           ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

		Use of Channel Modes:

			MODE #Finnish +im               ; Makes #Finnish channel moderated and
                                				'invite-only'.
			MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on
			MODE #Finnish +v Wiz            ; Allow WiZ to speak on #Finnish.
			MODE #Fins -s                   ; Removes 'secret' flag from channel
												#Fins.
			MODE #42 +k oulu                ; Set the channel key to "oulu".
			MODE #eu-opers +l 10            ; Set the limit for the number of users
												on channel to 10.
			MODE &oulu +b                   ; list ban masks set for channel.
			MODE &oulu +b *!*@*             ; prevent all users from joining.
			MODE &oulu +b *!*@*.edu         ; prevent any user from a hostname
												matching *.edu from joining.

	2. User modes

		Parameters: <nickname> {[+|-]|i|w|s|o}

		The user MODEs are typically changes which affect either how the
		client is seen by others or what 'extra' messages the client is sent.
		A user MODE command may only be accepted if both the sender of the
		message and the nickname given as a parameter are both the same.

		The available modes are as follows:

           i - marks a users as invisible;
           s - marks a user for receipt of server notices;
           w - user receives wallops;
           o - operator flag.

		Additional modes may be available later on.

		If a user attempts to make themselves an operator using the "+o"
		flag, the attempt should be ignored.  There is no restriction,
		however, on anyone `deopping' themselves (using "-o").  
		
		Numeric Replies:

			ERR_USERSDONTMATCH              RPL_UMODEIS
			ERR_UMODEUNKNOWNFLAG

		Use of user Modes:

			:MODE WiZ -w                    ; turns reception of WALLOPS messages
												off for WiZ.
			:Angel MODE Angel +i            ; Message from Angel to make themselves
												invisible.
			MODE WiZ -o                     ; WiZ 'deopping' (removing operator
												status).  The plain reverse of this
												command ("MODE WiZ +o") must not be
												allowed from users since would bypass
												the OPER command.
*/

// MODE #42 b
// [server:6667]:irc.local 367 pzgm #42 pzgm_!*@* pzgm :1640036076	
// :irc.local 367 pzgm #42 faffa!*@* pzgm :1640036231
// :irc.local 367 pzgm #42 lallo!*@* pzgm :1640036281
// :irc.local 368 pzgm #42 :End of channel ban list

void		cnl_mode( vector<string> args, User &u, Server &srv ) {

	string		usr_mode = u.getMode();
	string		knw_mode = "opsitnbv";
	Channel *	cnl;

	if (args[0][0] == '#')
		cnl = srv.getChannelByName( args[0] );
	else if (args[0][0] == '&')
		cnl = srv.getChannelByKey( &args[0][1] );
	else 
		cnl = NULL;
	
	if ( !cnl ) {
		send_error(u, ERR_NOSUCHCHANNEL, args[0]);
		return ;
	}

	ostringstream s;

	// List channel modes
	if ( args.size() == 1 ) {
		// irssi syntax [ :<srv_name> 324 <nickname> <channel> :+<ch_modes> ]
		s	<< ":" << srv.getName() << " 324 " << u.getNick() << " " << cnl->getName()
			<< " :+" << cnl->getMode() << "\r\n";
		
		send_reply(u.getFd(), s.str());

		return ;
	}

	string		cnl_mode = cnl->getMode();
	char 		flag = args[1][0];
	string 		mode = &args[1][1];

	for (size_t i = 0; i < mode.size(); i++) {
		if ( knw_mode.find(mode[i]) == string::npos ) {
			send_error(u, ERR_UNKNOWNMODE, args[0]);
			return ;
		}
	}

	if ( flag == '+' ) {
		for (size_t i = 0; i < mode.size(); i++) {
			if ( cnl_mode.find(mode[i]) == string::npos && mode[i] != 'o')
				cnl_mode += mode[i];
		}
	}
	else if ( flag == '-' ) {
		for (size_t i = 0; i < mode.size(); i++) {
			size_t to_remove = cnl_mode.find(mode[i]);
			if ( to_remove != string::npos )
				cnl_mode.erase(cnl_mode.begin() + to_remove);
		}
	}

	cnl->setMode(cnl_mode);

	// irssi syntax [ :<nickname>!<nickname>@<host> MODE <channel> :<mode> ]
	s	<< ":" << u.getNick() << "!" << u.getNick() << "@" << srv.getHost()
		<< " MODE " << args[0] << " :" << args[1] << "\r\n"; 

	send_reply(u.getFd(), s.str());
}

void		usr_mode( vector<string> args, User &u, Server &srv ) {

	string	usr_mode = u.getMode();
	string	knw_mode = "iswo";

	char flag = args[1][0];
	string mode = &args[1][1];

	if (u.getNick() != args[0]) {
		send_error(u, ERR_USERSDONTMATCH, args[0]);
		return ;
	}

	for (size_t i = 0; i < mode.size(); i++) {
		if ( knw_mode.find(mode[i]) == string::npos ) {
			send_error(u, ERR_UNKNOWNMODE, args[0]);
			return ;
		}
	}

	if ( flag == '+' ) {
		for (size_t i = 0; i < mode.size(); i++) {
			if ( usr_mode.find(mode[i]) == string::npos && mode[i] != 'o')
				usr_mode += mode[i];
		}
	}
	else if ( flag == '-' ) {
		for (size_t i = 0; i < mode.size(); i++) {
			size_t to_remove = usr_mode.find(mode[i]);
			if ( to_remove != string::npos )
				usr_mode.erase(usr_mode.begin() + to_remove);
		}
	}

	u.setMode(usr_mode);

	ostringstream s;

	// irssi syntax [ :<nickname>!<nickname>@<host> MODE <nickname> :<mode> ]
	s	<< ":" << u.getNick() << "!" << u.getNick() << "@" << srv.getHost()
		<< " MODE " << u.getNick() << " :" << args[1] << "\r\n"; 

	send_reply(u.getFd(), s.str());
}

void		mode( vector<string> args, User &usr, Server &srv ) {
	
	string mask = "#&!+";

	if (args.size() < 1) {
		send_error(usr, ERR_NEEDMOREPARAMS, "MODE");
		return ;
	}

	// find() search for full string to be match
	if ( mask.find_first_of(args[0]) != string::npos )
		cnl_mode(args, usr, srv);
	else
		usr_mode(args, usr, srv);

}
