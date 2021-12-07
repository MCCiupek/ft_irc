#include "headers.hpp"

/*
	Command: PONG
	Parameters: <daemon> [<daemon2>]

	PONG message is a reply to ping message.  If parameter <daemon2> is
	given this message must be forwarded to given daemon.  The <daemon>
	parameter is the name of the daemon who has responded to PING message
	and generated this message.

	Numeric Replies:

		ERR_NOORIGIN                    ERR_NOSUCHSERVER
*/

void		pong( vector<string> args, User &usr, Server &srv ) {

	if (args.size() == 1)
	{
		send_error(usr, ERR_NOORIGIN, args[0]);
		return ;
	}
	if (args[1].substr(1, args[1].size()) == srv.getHost() || args[1] == srv.getHost())
	{
		time(usr.getLastAct());
		usr.setPingStatus(false);
	}
}