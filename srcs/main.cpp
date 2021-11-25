#include "headers.hpp"

int main( int argc, char *argv[] ) {

    parser_res p;

    try {
        p = parser( argc, argv );
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    Server ircserv;

    if ( p.new_network ) {
        ircserv = Server(p.port, p.pwd, p.host, p.port_nwk, p.pwd_nwk);
    } else {
        ircserv = Server(p.port, p.pwd);
    }

    cout << ircserv << endl;

    return 0;
}