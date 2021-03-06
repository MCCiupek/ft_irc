#include "headers.hpp"

/*
	Command: NICK
	Parameters: <nickname>

	NICK command is used to give user a nickname or change the existing
	one.

	Numeric Replies:

		ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
		ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
		ERR_UNAVAILRESOURCE             ERR_RESTRICTED
*/

string		implode( vector<string>::iterator begin, vector<string>::iterator end )
{
	const char* const delim = " ";

	ostringstream str;
	copy(begin, end, ostream_iterator<string>(str, delim));

	return (str.str());
}

void		nick( vector<string> args, User &usr, Server &srv )
{
	ostringstream s;

	if (args.size() == 0)
	{
		send_error(usr, ERR_NEEDMOREPARAMS, "NICK");
		return ;
	}
	else if (args[0].empty() || args[0].length() > MAX_USR_NICK_LEN || args.size() > 1)
	{
		send_error(usr, ERR_ERRONEUSNICKNAME, implode(args.begin() + 1, args.end()));
		return ;
	}
	if (srv.is_registered(usr) && usr.getNick() == args[0])
		return ;

	vector<User*>	usrs = srv.getUsers();

	for (vector<User*>::iterator it = usrs.begin(); it != usrs.end(); it++)
		if ((*it)->getNick() == args[0])
		{
			send_error(usr, ERR_NICKNAMEINUSE, args[0]);
			return ;
		}

	if (srv.is_registered(usr))
	{
		if (usr.getNick().empty()) 
			cout << MAGENTA << "User #" << usr.getFd() << " nick set to " << args[0] << RESET << endl;
		else	
			cout << MAGENTA << usr.getNick() << ": Nick changed to " << args[0] << RESET << endl;
		if (usr.getIsSet() && usr.getNick().empty())
		{
			if (srv.getPassword() != "")
				if (!check_password(usr, srv))
					return;

			cout << GREEN << "User #" << usr.getFd() << " registred as " << args[0] << RESET << endl;
			usr.setNick(args[0]);
			messageoftheday(srv, usr);
		}
		send_notice(usr, usr, NTC_NICK(args[0]));
		usr.setNick(args[0]);
	}
}
