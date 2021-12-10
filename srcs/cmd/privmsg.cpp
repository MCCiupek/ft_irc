#include "headers.hpp"

/*
	Command: PRIVMSG
	Parameters: <receiver>{,<receiver>} <text to be sent>

	PRIVMSG is used to send private messages between users.  <receiver>
	is the nickname of the receiver of the message.  <receiver> can also
	be a list of names or channels separated with commas.

	The <receiver> parameter may also be a host mask  (#mask)  or  server
	mask  ($mask).   In  both cases the server will only send the PRIVMSG
	to those who have a server or host matching the mask.  The mask  must
	have at  least  1  (one)  "."  in it and no wildcards following the
	last ".".  This requirement exists to prevent people sending messages
	to  "#*"  or "$*",  which  would  broadcast  to  all  users; from
	experience, this is abused more than used responsibly and properly.
	Wildcards are  the  '*' and  '?'   characters.   This  extension  to
	the PRIVMSG command is only available to Operators.

	Numeric Replies:

		ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
		ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
		ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
		ERR_NOSUCHNICK
		RPL_AWAY
*/

void		privmsg( vector<string> args, User &usr, Server &srv ) {

	string 	mask = "$#";
	User *	receiver;
	
	if (args.size() < 2) {
		send_error(usr, ERR_NORECIPIENT, args[0]);
		return ;
	}
	if (args.size() == 2) {
		send_error(usr, ERR_NOTEXTTOSEND, args[0]);
		return ;
	}
	if ( mask.find(args[1][0], 0) == string::npos ) {
		receiver = srv.getUserByNick(args[1]);
		if ( !receiver ) {
			send_error(usr, ERR_NOSUCHNICK, args[0]);
			return ;
		}
		string msg = args[2] + "\n";
		send(receiver->getFd(), &msg, msg.length(), 0);
	}
	else {
		// TODO: find users by server/channel and loop over matching users.
		// Check that usr is oper.
	}
}