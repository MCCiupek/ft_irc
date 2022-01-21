#include "headers.hpp"

int main( int argc, char *argv[] ) {

	map<string, string> p;

	define_errors();

	try {
		p = parser( argc, argv );
		Server ircserv;
		if ( p.size() > 2 ) {
			ircserv = Server(p["PORT"], p["PWD"], p["HOST"], p["MOTD"], p["OPER"]);
		} else {
			ircserv = Server(p["PORT"], p["PWD"]);
		}
		ircserv.initConn();
		ircserv.run();
	}
	catch (const exception& e) {
		cerr << BOLDRED << e.what() << RESET << endl;
		return errno;
	}

	return 0;
}
