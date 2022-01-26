#include "headers.hpp"

int main( int argc, char *argv[] ) {

	map<string, string> p;

	define_errors();

	try {
		p = parser( argc, argv );
		// Had to copy initConn() and run() two times because of the scope
		if ( p.size() > 2 ) {
			Server ircserv(p["PORT"], p["PWD"], p["HOST"], p["MOTD"], p["OPER"]);
			ircserv.initConn();
			ircserv.run();
		}
		else {
			Server ircserv(p["PORT"], p["PWD"]);
			ircserv.initConn();
			ircserv.run();	
		}
	}
	catch (const exception& e) {
		cerr << BOLDRED << e.what() << RESET << endl;
		return errno;
	}

	return 0;
}
