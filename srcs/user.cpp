#include "headers.hpp"
#include "User.hpp"

/*
	Command: USER
	Parameters: <username> <hostname> <servername> <realname>

	The USER message is used at the beginning of connection to specify
	the username, hostname, servername and realname of s new user.  It is
	also used in communication between servers to indicate new user
	arriving on IRC, since only after both USER and NICK have been
	received from a client does a user become registered.

	Numeric Replies:

        ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
*/

void	user( vector<string> args, User &usr, Server &srv )
{
	if (args.size() < 5)
	{
		send_error(usr, ERR_NEEDMOREPARAMS, args[0]);
		return ;
	}
	if (usr.getIsSet())
	{
		send_error(usr, ERR_ALREADYREGISTRED, usr.getNick());
		return ;
	}

	usr.setUsername(args[1]);
	usr.setHostname(args[2]);
	usr.setServername(args[3]);
	usr.setRealName(args[4]);
	usr.setIsSet(true);

	if (!usr.getNick().empty())
	{
		cout << GREEN << "User #" << usr.getFd() << " registred as " << usr.getNick() << RESET << endl;
		messageoftheday(srv, usr);
	}
}