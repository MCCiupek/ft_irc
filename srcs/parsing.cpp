#include "headers.hpp"

map<string, string> parser( int n_params, char *params[] ) {

	map<string, string> res;

	if ( n_params != 4 && n_params != 3 ) {
		throw eExc("Usage: ./ircserv [host:port_network:password_network] <port> <password>");
	}

	res["port"] = params[1 + (n_params == 4)];
	res["pwd"] = params[2 + (n_params == 4)];

	if (n_params == 4) {

		vector<string> v = ft_split(params[1], ":");
		if ( v.size() != 3 )
			throw eExc("Usage: ./ircserv [host:port_network:password_network] <port> <password>");

		res["host"] = v[0];
		res["port_nwk"] = v[1];
		res["pwd_nwk"] = v[2];
	}

	return res;
}

string				to_upper( string s )
{
	for (string::iterator it = s.begin(); it != s.end(); it++)
		*it -= 32;
	
	return (s);
}

int				parsing( vector<string> args, User &usr, Server &srv )
{
	//(void)usr;
	//(void)srv;

	transform(args[0].begin(), args[0].end(), args[0].begin(), ::toupper);

	// Remove \n at the end of command
	if (args.size() == 1)
		args[0].pop_back();
	
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

	//cout << usr.getFd() << endl;
	//cout << m << endl;

	// Call function
	if ( m.count(args[0]) > 0 ) {
		m[args[0]](args, usr, srv);
		return 1;
	}

	return 0;
}