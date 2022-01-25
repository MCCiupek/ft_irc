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
	//if (!is_upper(cmd))
	//	cmd = to_upper(cmd);

	// Remove \n at the end of command
	//if (args.size() == 1)
	//	cmd.erase(cmd.end() - 1);
	
	for (size_t i = 0; i < args.size(); i++) {
		// if (back(args[i]) == '\r')
		// 	cout << "\\r" << endl;
		// else
		// 	cout << back(args[i]) << endl;
		if (back(args[i]) == '\r')
			args[i] = pop_back(args[i]);
		// if (back(args[i]) == '\r')
		// 	cout << "\\r" << endl;
		// else
		// 	cout << back(args[i]) << endl;
	}
		// if (args[i].back() == '\r')
		// 	args[i].pop_back();

	// for (size_t i = 0; i < args.size(); i++) {
	// 	for (size_t j = 0; j < args[i].size(); j++) {
	// 		if (args[i][j] == '\n')
	// 			cout << YELLOW << "\\n" << RESET;
	// 		else if (args[i][j] == '\r')
	// 			cout << YELLOW << "\\r" << RESET;
	// 		else
	// 			cout << YELLOW << args[i][j] << RESET;
	// 	}
	// 	cout << " ";
	// }
	// cout << endl;

	
	//cout << cmd << endl;
	//args[args.size() - 1].erase(args[args.size() - 1].end() - 1);

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