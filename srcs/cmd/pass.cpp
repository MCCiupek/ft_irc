#include "headers.hpp"

/*
	Command: PASS
	Parameters: <password>

	The PASS command is used to set a 'connection password'.  The
	password can and must be set before any attempt to register the
	connection is made.  Currently this requires that clients send a PASS
	command before sending the NICK/USER combination and servers *must*
	send a PASS command before any SERVER command.  The password supplied
	must match the one contained in the C/N lines (for servers) or I
	lines (for clients).  It is possible to send multiple PASS commands
	before registering but only the last one sent is used for
	verification and it may not be changed once registered.  Numeric
	Replies:

			ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

	Example:

			PASS secretpasswordhere
*/

void		pass( vector<string> args, User &usr, Server &srv ) {

	(void)srv;

	if ( args.size() < 1 ) {
		send_error( usr, ERR_NEEDMOREPARAMS, "PASS" );
		return ;
	}
	if ( usr.isRegistered() ) {
		send_error(usr, ERR_ALREADYREGISTRED, usr.getNick());
		return ;
	}

	usr.setPasswd(args[0]);
	cout << RED << "User #" << usr.getFd() << " password added" << RESET << endl;
}