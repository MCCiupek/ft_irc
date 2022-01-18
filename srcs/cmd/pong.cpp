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

	if (args.size() == 0)
	{
		send_error(usr, ERR_NOORIGIN, "PONG");
		return ;
	}
	if (args[0].substr(1, args[0].size()) == srv.getHost() || args[0] == srv.getHost())
	{
		time(usr.getLastAct());
		usr.setPingStatus(false);
	}
}