#include "headers.hpp"

int main( int argc, char *argv[] ) {

	map<string, string> p;

	define_errors();

	try {
		p = parser( argc, argv );

		Server ircserv;
		if ( p.size() > 2 ) {
			ircserv = Server(p["port"], p["pwd"], p["host"], p["port_nwk"], p["pwd_nwk"]);
		} else {
			ircserv = Server(p["port"], p["pwd"]);
		}
		//cout << ircserv;
		ircserv.initConn();
		ircserv.run();
	}
	catch (const exception& e) {
		cerr << BOLDRED << "Error: " << RESET << e.what() << endl;
		return errno;
	}

	return 0;
}
