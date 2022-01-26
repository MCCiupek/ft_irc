#include "headers.hpp"

/*
	Command: OPER
	Parameters: <user> <password>

	OPER message is used by a normal user to obtain operator privileges.
	The combination of <user> and <password> are required to gain
	Operator privileges.

	If the client sending the OPER command supplies the correct password
	for the given user, the server then informs the rest of the network
	of the new operator by issuing a "MODE +o" for the clients nickname.

	The OPER message is client-server only.

	Numeric Replies:

			ERR_NEEDMOREPARAMS              RPL_YOUREOPER
			ERR_NOOPERHOST                  ERR_PASSWDMISMATCH

	Example:

	OPER foo bar                    ; Attempt to register as an operator
									using a username of "foo" and "bar" as
									the password.
*/

void		oper( vector<string> args, User &usr, Server &srv )
{
	if ( args.size() < 2 ) {
		send_error( usr, ERR_NEEDMOREPARAMS, "OPER" );
		return ;
	}

	if ( srv.getIRCOperators().size() == 0 ) {	
		send_error( usr, ERR_NOOPERHOST, "OPER");
		return ;
	}

	if ( srv.username_isIRCOper(args[0]) )
	{
		if ( !srv.isIRCOperator(args[0], args[1]) ) {
			send_error( usr, ERR_PASSWDMISMATCH, "OPER" );
			return ;
		}
		usr.setIsIRCOper(true);
		usr.addMode("o");
		send_reply( usr, 381, ":You are now an IRC operator\r\n");
	}
}