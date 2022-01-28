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
		   [p - private channel flag;]
			s - secret channel flag;
			i - invite-only channel flag;
			t - topic settable by channel operator only flag;
		   [n - no messages to channel from clients on the outside;]
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

string		add_cnl_mode( string mode, vector<string> args, Channel *cnl, User &u, Server &srv ) {

	User *		target_usr;
	string		arg_mode = "olvk";
	string		cnl_mode;

	cnl_mode = cnl->getMode();

	for (size_t i = 0; i < mode.size(); i++) {
		if ( arg_mode.find(mode[i]) != string::npos && args.size() < 3 ) {
			send_error(u, ERR_NEEDMOREPARAMS, args[0]);
			return "x";
		}
		if ( mode[i] == 'o' ) { 
			// grant oper priviledge to user in arg
			target_usr = srv.getUserByNick(args[2]);
			if ( !target_usr ) {
				send_error(u, ERR_NOSUCHNICK, args[2]);
				return "x";
			}
			cnl->addOper( target_usr );
		} else if ( mode[i] == 'l' ) { 
			// set user limit with arg
			cnl->setLimit(atoi(args[2].c_str()));
		} else if ( mode[i] == 'b' && args.size() > 2 ) { 
			// set ban mask
			cnl->ban(args[2]);
		} else if ( mode[i] == 'v' ) { 
			// if chan is moderated give ability to speak to user in arg
			if ( !cnl->isModerated() )
				return "x";
			target_usr = srv.getUserByNick(args[2]);
			if ( !target_usr ) {
				send_error(u, ERR_NOSUCHNICK, args[2]);
				return "x";
			}
			cnl->addModerator( target_usr );
		} else if ( mode[i] == 'k' ) { 
			// change key with arg
			if ( cnl->getHasKey() ) {
				send_error(u, ERR_KEYSET, args[2]);
				return "x";
			}
			cnl->setKey(args[2]);
		}
		if ( cnl_mode.find(mode[i]) == string::npos)
			cnl_mode += mode[i];
	}
	return cnl_mode;
}

string	remove_cnl_mode( string mode, vector<string> args, Channel *cnl, User &u, Server &srv ) {

	User *		target_usr;
	string		arg_mode = "obv";
	string		cnl_mode;

	cnl_mode = cnl->getMode();

	for (size_t i = 0; i < mode.size(); i++) {
		if ( arg_mode.find(mode[i]) != string::npos && args.size() < 3) {
			send_error(u, ERR_NEEDMOREPARAMS, args[0]);
			return "x";
		}
		size_t to_remove = cnl_mode.find(mode[i]);
		if ( mode[i] == 'o' ) { 
			// take oper priv from user in arg
			target_usr = srv.getUserByNick(args[2]);
			if ( !target_usr ) {
				send_error(u, ERR_NOSUCHNICK, args[2]);
				return "x";
			}
			cnl->deleteOper( target_usr );
		} else if ( mode[i] == 'l' ) {
			// unset user limit with arg
			cnl->setLimit(MAX_USR_PER_CHAN);
		} else if ( mode[i] == 'b' ) { 
			// delete ban mask given in arg (if on)
			cnl->unban(args[2]);
		} else if ( mode[i] == 'v' ) { 
			// if chan is moderated take ability to speak from user in arg
			if ( !cnl->isModerated() )
				return "x";
			target_usr = srv.getUserByNick(args[2]);
			if ( !target_usr ) {
				send_error(u, ERR_NOSUCHNICK, args[2]);
				return "x";
			}
			cnl->deleteModerator( target_usr );
		} else if ( mode[i] == 'k' ) {
			cnl->unsetKey();
		}
		if ( to_remove != string::npos ) {
			cnl_mode.erase(cnl_mode.begin() + to_remove);
		}
	}
	return cnl_mode;
}

void		cnl_mode( vector<string> args, User &u, Server &srv ) {

	char 		flag;
	string 		mode;
	string		usr_mode = u.getMode();
	Channel *	cnl = srv.getChannelByName( args[0] );
	string		knw_mode = AVAILABLE_CHANNEL_MODES;
	string		arg_mode = "oblvk";
	string		cnl_mode;

	// Check channel
	if ( !cnl )
		return send_error(u, ERR_NOSUCHCHANNEL, args[0]);

	if ( !cnl->isOnChann(u) )
		return send_error(u, ERR_NOTONCHANNEL, args[0]);

	// List channel modes
	if ( args.size() == 1 ) {
		send_reply(u, 324, RPL_CHANNELMODEIS(cnl->getName(), cnl->getMode()));
		send_reply(u, 329, RPL_CREATIONTIME(cnl->getName(), cnl->getCreationDate()));
		return ;
	}

	// Print ban mask list
	if ( args[1] == "b" || (args[1] == "+b" && args.size() < 3) ) {
		vector<string> b_list = cnl->getBanMask();
		for (size_t j = 0; j < b_list.size(); j++)
			send_reply(u, 367, RPL_BANLIST(cnl->getName(), b_list[j]));
		send_reply(u, 368, RPL_ENDOFBANLIST(cnl->getName()));
		return;
	}

	// Parse args
	if (args[1][0] != '+' && args[1][0] != '-') {
		flag = ' ';
		mode = args[1];
	} else {
		flag = args[1][0];
		mode = &args[1][1];
	}

	cnl_mode = cnl->getMode();

	// Check user's priv: mode change (flag={+,-}) is only authorized to channel operators
	if ( flag != ' ' && !cnl->isOper(u) )
		return send_error(u, ERR_CHANOPRIVSNEEDED, args[0]);
	
	// When using the 'o' and 'b' options, a restriction on a total of three per mode command has been imposed.
	if ( flag != ' ' && (mode.find('o') || mode.find('b')) && mode.size() > 3 )
		return send_error(u, ERR_CHANOPRIVSNEEDED, args[0]);

	// Check modes
	for (size_t i = 0; i < mode.size() - 1; i++)
		if ( knw_mode.find(mode[i]) == string::npos )
			return send_error(u, ERR_UNKNOWNMODE, &mode[i]);

	// Change mode
	if ( flag == '+' ) {
		cnl_mode = add_cnl_mode(mode, args, cnl, u, srv);
		if (cnl_mode == "x")
			return;
	} else if ( flag == '-' ) {
		cnl_mode = remove_cnl_mode(mode, args, cnl, u, srv);
		if (cnl_mode == "x")
			return;
	}
	
	cnl->setMode(cnl_mode);
	if (args.size() > 2)
		send_notice_channel(u, cnl, NTC_CHANMODE_ARG(cnl->getName(), args[1], args[2]));
	else
		send_notice_channel(u, cnl, NTC_CHANMODE(cnl->getName(), args[1]));
}

void		usr_mode( vector<string> args, User &u, Server &srv ) {

	string	usr_mode = u.getMode();
	string	knw_mode = AVAILABLE_USER_MODES;

	(void)srv;

	if (args.size() < 1)
		return send_error(u, ERR_NEEDMOREPARAMS, "MODE");

	if (args.size() < 2)
		return send_reply(u, 221, RPL_UMODEIS(u.getMode()));

	char flag = args[1][0];
	string mode = args[1].substr(1, args[1].length() - 1);

	if (u.getNick() != args[0]) {
		send_error(u, ERR_USERSDONTMATCH, "");
		return ;
	}

	for (size_t i = 0; i < mode.size(); i++) {
		if ( knw_mode.find(mode[i]) == string::npos ) {
			send_error(u, ERR_UNKNOWNMODE, &mode[i]);
			return ;
		}
	}

	string	to_add = "";

	if ( flag == '+' ) {
		for (size_t i = 0; i < mode.size(); i++) {
			cout << mode[i] << endl;
			if (( usr_mode.find(mode[i]) == string::npos) && mode[i] != 'o')
				to_add += mode[i];
			else if (mode[i] == 'o' && u.isIRCOper()) // IRC operator
				to_add += mode[i];
		}
	}
	else if ( flag == '-' ) {
		for (size_t i = 0; i < mode.size(); i++) {
			size_t to_remove = usr_mode.find(mode[i]);
			if ( to_remove != string::npos )
				usr_mode.erase(usr_mode.begin() + to_remove);
		}
	}
	else
		return send_error(u, ERR_UMODEUNKNOWNFLAG, args[1]);

	if (to_add.size()) {
		u.addMode(to_add);
		send_notice(u, u, NTC_MODE(u.getNick(), flag + to_add));
	}
}

void		mode( vector<string> args, User &usr, Server &srv )
{
	
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
