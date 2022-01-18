#include "headers.hpp"

/*
	Command: PING
	Parameters: <server1>

	The PING message is used to test the presence of an active client at
	the other end of the connection.  A PING message is sent at regular
	intervals if no other activity detected coming from a connection.  If
	a connection fails to respond to a PING command within a set amount
	of time, that connection is closed.

	Any client which receives a PING message must respond to <server1>
	(server which sent the PING message out) as quickly as possible with
	an appropriate PONG message to indicate it is still there and alive.
	Servers should not respond to PING commands but rely on PINGs from
	the other end of the connection to indicate the connection is alive.
	If the <server2> parameter is specified, the PING message gets
	forwarded there.

	Numeric Replies:

		ERR_NOORIGIN                    ERR_NOSUCHSERVER
*/

void		ping( vector<string> args, User &usr, Server &srv ) {

	if (args.size() < 1)
	{
		send_error(usr, ERR_NOORIGIN, "PING");
		return ;
	}
	else if (args.size() > 1)
	{
		send_error(usr, ERR_NOSUCHSERVER, args[1]);
		return ;
	}
	string reply = ":" + srv.getHost() + " PONG " + srv.getHost() + " " + args[0] + "\r\n";
	send(usr.getFd(), reply.c_str(), reply.length(), 0);
}