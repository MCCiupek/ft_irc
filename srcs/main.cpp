#include "headers.hpp"

int main( int argc, char *argv[] ) {

    map<string, string> p;

    try {
        p = parser( argc, argv );

        Server ircserv;
        if ( p.size() > 2 ) {
            ircserv = Server(p["port"], p["pwd"], p["host"], p["port_nwk"], p["pwd_nwk"]);
        } else {
            ircserv = Server(p["port"], p["pwd"]);
        }
        cout << ircserv;
        ircserv.initConn();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return errno;
    }

    return 0;
}