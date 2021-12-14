#include "headers.hpp"

map<string, string>		parser( int n_params, char *params[] ) {

	map<string, string> res;

	if ( n_params != 4 && n_params != 3 && n_params != 2) {
		throw eExc("Usage: ./ircserv [host:port_network:password_network] <port> <password> \
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

	transform(args[0].begin(), args[0].end(), args[0].begin(), ::toupper);
	//if (!is_upper(args[0]))
	//	args[0] = to_upper(args[0]);

	// Remove \n at the end of command
	if (args.size() == 1)
		args[0].erase(args[0].end() - 1);
		// args[0].pop_back(); --> C++11
	
	//cout << args[0] << endl;

	map<string, FnPtr>	m;

	m["NICK"] = nick;
	m["USER"] = user;
	m["PING"] = ping;
	m["PONG"] = pong;
	m["JOIN"] = join;
	m["MODE"] = ping;
	m["WHO"] = who;
	m["PRIVMSG"] = privmsg;

	// Call function
	if ( m.count(args[0]) > 0 ) {
		m[args[0]](args, usr, srv);
		return 1;
	}

	return 0;
}