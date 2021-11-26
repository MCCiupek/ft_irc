#include "headers.hpp"

map<string, string> parser( int n_params, char *params[] ) {

    map<string, string> res;

    if ( n_params != 4 && n_params != 3 ) {
        throw eExc("Usage: ./ircserv [host:port_network:password_network] <port> <password>");
    }

    /*
    All ports below 1024 are RESERVED (unless you’re the superuser)! 
    You can have any port number above that, right up to 65535 
    (provided they aren’t already being used by another program).
    */
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