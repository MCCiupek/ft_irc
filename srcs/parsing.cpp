#include "headers.hpp"

map<string, string> parser( int n_params, char *params[] ) {

    map<string, string> res;

    if ( n_params != 4 && n_params != 3 ) {
        throw eUsage();
    }

    res["port"] = params[1 + (n_params == 4)];
    res["pwd"] = params[2 + (n_params == 4)];

    if (n_params == 4) {

        vector<string> v = ft_split(params[1], ":");
        if ( v.size() != 3 )
            throw eUsage();

        res["host"] = v[0];
        res["port_nwk"] = v[1];
        res["pwd_nwk"] = v[2];
    }

    return res;
}