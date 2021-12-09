#include "headers.hpp"

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

static int		check_param( string param ) {

	if (param[0] == '#' && param.find("*", 0) != string::npos)
		return 1;
	if (count(param.begin(), param.end(), '@') > 1 || count(param.begin(), param.end(), '!') > 1)
		return 1;
	size_t host_pos = param.find("@", 0);
	size_t user_pos = param.find("!", 0);
	if (host_pos == string::npos || user_pos == string::npos)
		return 1;
	if (user_pos > host_pos)
		return 1;
	return 0;
}

static bool		has_permission(const string &query, User &usr, User &cli, Server &srv)
{
	string nick;
	string user;
	string host;
	(void)srv;

	get_infos(query, nick, user, host);
	if ((ft_match(cli.getNick(), nick) && ft_match(cli.getUsername(), user) && ft_match(cli.getHostname(), host))
		&& ((cli.getIsVisible() == true || /*user_on_same_channel(*cli, *usr) ||*/ &cli == &usr)
				|| usr.getIsOper() == true))
		return true;
	return false;
}

static int	who_client(const std::string &query, User &usr, Server &srv, char c) {

	map<int, User>			cli_list = srv.getUsers();

	for ( map<int, User>::iterator it = cli_list.begin(); it != cli_list.end(); ++it ) {
		User cli = it->second;
		if ( c == 'o' && cli.getIsOper() )
			continue ;
		else if ( has_permission(query, usr, cli, srv) == true ) {
			
			string reply;

			reply += string("*") + " ";
			reply += cli.getUsername() + " ";
			reply += cli.getHostname() + " ";
			reply += srv.getHost() + " ";
			reply += cli.getNick() + " ";
			if ( cli.getIsAway() )
				reply += "G";
			else
				reply += "H";
			if ( cli.getIsOper() )
				reply += "*";
			reply += " ";
			reply += string(":0") + " ";
			reply += cli.getRealName();
		
			send(usr.getFd(), reply.c_str(), reply.length(), 0);
		}
	}
	return 1;
}

int				who_channel(const std::string &query, User &usr, Server &srv, char c) {
	(void)query;
	(void)usr;
	(void)srv;
	(void)c;
	return 0;
}

void		who( vector<string> args, User &usr, Server &srv ) {

	string	mask = "#&!+";
	char	opt = 'n';

	if (args.size() < 2)
		return ;
	else if (check_param(args[1]))
		return ;
	if (args.size() >= 3 && args[2].size() == 1 && args[2][0] == 'o')
		opt = 'o';
	if (mask.find(args[1][0], 0) != string::npos) {
		who_channel(args[1], usr, srv, opt);
	} else {
		who_client(args[1], usr, srv, opt);
	}
}