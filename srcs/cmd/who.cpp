#include "headers.hpp"

/*
	Syntax:
		WHO [ <nick> | <channel> | **]

	Parameters:
	    The channel name or output filter; if no arguments are given, the current
	    channel will be used.

	Description:
	    Displays information about users in the specified channel. If you specify a
	    filter, all the users whose nick, userhost or realname matches the filter
	    will be returned.
	    If the channel is secret and you are not on it, you will not receive any
	    output.
	    Common flags:
	        H:    The user is available.
	        G:    The user is away.
	        *:    The user is an IRC operator.
	        @:    The user is a channel operator.
	        +:    The user is a channel voice.
	
	Examples:
	    /WHO
	    /WHO #irssi
	    /WHO bob
	    /WHO sar*
*/

/*
	Command: WHO
	Parameters: [<name> [<o>]]

	The WHO message is used by a client to generate a query which returns
	a list of information which 'matches' the <name> parameter given by
	the client.  In the absence of the <name> parameter, all visible
	(users who aren't invisible (user mode +i) and who don't have a
	common channel with the requesting client) are listed.  The same
	result can be achieved by using a <name> of "0" or any wildcard which
	will end up matching every entry possible.

	The <name> passed to WHO is matched against users' host, server, real
	name and nickname if the channel <name> cannot be found.

	If the "o" parameter is passed only operators are returned according
	to the name mask supplied.

	Numeric Replies:

		ERR_NOSUCHSERVER
		RPL_WHOREPLY                    RPL_ENDOFWHO
	

	RPL_WHOREPLY: "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"
	RPL_ENDOFWHO: "<name> :End of /WHO list"

	- The RPL_WHOREPLY and RPL_ENDOFWHO pair are used to answer a WHO message. 
	The RPL_WHOREPLY is only sent if there is an appropriate match to the WHO query. 
	If there is a list of parameters supplied with a WHO message, a RPL_ENDOFWHO 
	must be sent after processing each list item with <name> being the item.
	If the name did not match a channel, the server may set <channel> to any 
	channel of which <nick> is a member. If the <nick> is not a member of any channel, 
	<channel> should be set to '*'.

	If no name was given in the command, <name> must be set to '*' for RPL_ENDOFWHO.

	<H|G> denotes whether the user is "here", or "gone". A user is gone if he has 
	indicated so via the AWAY message.

	15-441 Notes:
		1. <H|G> should always be set to 'H'.
		2. Omit [*].
		3. Omit [@|+].
		4. In response to a query for a list of channels, send ONE RPL_WHOREPLY, 
		   of the form: :servername 315 user #c1,#c2 :End of /WHO list.
*/

static bool		has_permission(const string &query, User &usr, User &cli, Server &srv)
{
	string nick;
	string user;
	string host;
	(void)query;
	(void)usr;
	(void)cli;
	(void)srv;

	// get_infos(query, nick, user, host);
	// if ((ft_match(cli.getNick(), nick) && ft_match(cli.getUsername(), user) && ft_match(cli.getHostname(), host))
	// 	&& ((cli.isVisible() == true || /*user_on_same_channel(*cli, *usr) ||*/ &cli == &usr)
	// 			|| usr.isOper() == true))
	return true;
	// return false;
}

static int		who_user( const vector<string> args, User &usr, Server &srv, bool wild )
{
	ostringstream	s;

	// Works only if is only <name> or <name> + <channel> (<channel> is ignored)
	if ( args.size() == 1 || (args.size() == 2 && args[1][0] == '#') )
	{
		map<int, User>	users = srv.getUsers();

		for ( map<int, User>::iterator it = users.begin(); it != users.end(); ++it )
		{
			User u = it->second;

			// irssi syntax :<server> 352 <user> <*|u.curr_channel> <u.realname> <u.hostname> <u.servername>
			//									 <u.nickname> <H|G>[*][@|+] :<hopcount> <u.realname>
			if ( (u.getHostname() == args[0] || u.getServername() == args[0] || u.getRealName() == args[0] ) &&
				has_permission(args[0], usr, u, srv) == true )
			{
				s	<< ":" << srv.getName() << " 352 " << usr.getNick() << " "
					<< (u.getCurrChan() == nullptr ? "*" : u.getCurrChan()->getName())
					<< " " << u.getRealName() << " " << u.getHostname() << " "
					<< u.getServername() << " " << u.getNick() << " " << (u.getIsAway() ? "G" : "H")
					<< (u.isIRCOper() ? "*" : "") << (u.isChanOper() ? "@" : "") << " " << ":0 "
					<< u.getRealName() << "\r\n";
			}
		}
	}

	// irssi syntax :<server> 315 <nickname> <who param> :End of /WHO list.
	s 	<< ":" << srv.getName() << " 315 " << usr.getNick() << " " << (wild == true ? "*" : args[0])
		<< " :End of /WHO list." << "\r\n";

	send_reply(usr.getFd(), s.str());
	
	return 1;
}

int				who_channel( const vector<string> args, User &usr, Server &srv, bool wild ) {
	
	ostringstream	s;

	// Works only if is only <channel> or <channel> + <channel2> (<channel2> is ignored)
	if ( args.size() == 1 || (args.size() == 2 && args[1][0] == '#') )
	{
		map<int, User>	users = srv.getUsers();

		for ( map<int, User>::iterator it = users.begin(); it != users.end(); ++it )
		{
			User u = it->second;

			// irssi syntax :<server> 352 <user> <*|u.curr_channel> <u.realname> <u.hostname> <u.servername>
			//									 <u.nickname> <H|G>[*][@|+] :<hopcount> <u.realname>
			s	<< ":" << srv.getName() << " 352 " << usr.getNick() << " "
				<< (u.getCurrChan() == nullptr ? "*" : u.getCurrChan()->getName())
				<< " " << u.getRealName() << " " << u.getHostname() << " "
				<< u.getServername() << " " << u.getNick() << " " << (u.getIsAway() ? "G" : "H")
				<< (u.isIRCOper() ? "*" : "") << (u.isChanOper() ? "@" : "") << " " << ":0 "
				<< u.getRealName() << "\r\n";
		}
	}

	// irssi syntax :<server> 315 <nickname> <who param> :End of /WHO list.
	s 	<< ":" << srv.getName() << " 315 " << usr.getNick() << " " << (wild == true ? "*" : args[0])
		<< " :End of /WHO list." << "\r\n";

	send_reply(usr.getFd(), s.str());

	return 1;
}

int				who_wildcard( vector<string> args, User &usr, Server &srv)
{
	Channel		*chan = usr.getCurrChan();

	if (chan != nullptr)
	{
		args[0] = chan->getName();
		who_channel(args, usr, srv, true);
	}
	else
	{
		args[0] = usr.getNick();
		who_user(args, usr, srv, true);
	}

	return 1;
}

void			who( vector<string> args, User &usr, Server &srv ) {

	// TODO: handle multiple <name> calls
	// char	opt = 'n';

	if (args.size() == 0)
	{
		send_error(usr, ERR_NEEDMOREPARAMS, "WHO");
		return ;
	}

	// // ope
	// if (args.size() >= 3 && args[1].size() == 1 && args[1][0] == 'o')
	// 	opt = 'o';

	if (args[0][0] == '#')
		who_channel(args, usr, srv, false);
	else if (args[0] == "*")
		who_wildcard(args, usr, srv);
	else
		who_user(args, usr, srv, false);
}
