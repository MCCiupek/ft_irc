#include "headers.hpp"

map<string, string>		parser( int n_params, char *params[] ) {

	map<string, string> res;

	if ( n_params != 4 && n_params != 3 && n_params != 2) {
		throw eExc("Usage: ./ircserv [host:port_network:password_network] <port> <password> \n\
		        ./ircserv <txt.conf>");
	}

	if (n_params == 2)
		return (conf_file(params[1]));

	res["PORT"] = params[1 + (n_params == 4)];
	res["PWD"] = params[2 + (n_params == 4)];

	if (n_params == 4) {

		vector<string> v = ft_split(params[1], ":");
		if ( v.size() != 3 )
			throw eExc("Usage: ./ircserv [host:port_network:password_network] <port> <password>");

		res["HOST"] = v[0];
		res["PORT_NWK"] = v[1];
		res["PWD_NWK"] = v[2];
	}

	return res;
}

int						parsing( vector<string> args, User &usr, Server &srv )
{
	string	cmd = args[0];

	transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	//if (!is_upper(cmd))
	//	cmd = to_upper(cmd);

	// Remove \n at the end of command
	//if (args.size() == 1)
	//	cmd.erase(cmd.end() - 1);
	
	cout << cmd << endl;

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
	m["TOPIC"] = topic;
	m["NAMES"] = names;

	// Call function
	if ( m.count(cmd) > 0 ) {
		args.erase(args.begin());	// Remove args[0] (command)
		m[cmd](args, usr, srv);
		return 1;
	}

	return -1;
}