#include "headers.hpp"

map<string, string>		parser( int n_params, char *params[] ) {

	map<string, string> res;

	if ( n_params != 3 && n_params != 2) {
		throw eExc("Usage: ./ircserv <port> <password> \n\
		        ./ircserv <txt.conf>");
	}

	if (n_params == 2)
		return (conf_file(params[1]));

	res["PORT"] = params[1];
	res["PWD"] = params[2];

	return res;
}

int						parsing( vector<string> args, User &usr, Server &srv )
{
	string	cmd = args[0];

	transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	
	for (size_t i = 0; i < args.size(); i++) 
		if (back(args[i]) == '\r')
			args[i] = pop_back(args[i]);

	map<string, FnPtr>	m;

	m["NICK"] = nick;
	m["USER"] = user;
	m["MODE"] = mode;
	m["PING"] = ping;
	m["PONG"] = pong;
	m["JOIN"] = join;
	m["MODE"] = mode;
	m["WHO"] = who;
	m["PRIVMSG"] = privmsg;
	m["PART"] = part;
	m["PASS"] = pass;
	m["TOPIC"] = topic;
	m["NAMES"] = names;
	m["QUIT"] = quit;
	m["KICK"] = kick;
	m["NOTICE"] = notice;
	m["INVITE"] = invite;
	m["OPER"] = oper;

	// Call function
	if ( m.count(cmd) > 0 ) {
		args.erase(args.begin());	// Remove args[0] (command)
		m[cmd](args, usr, srv);
		return 1;
	}

	return -1;
}