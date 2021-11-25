#include "headers.hpp"

parser_res parser( int n_params, char *params[] ) {

    parser_res res;

    res.new_network = n_params == 4 ? 1 : 0;

    if ( n_params != 4 && n_params != 3 ) {
        throw eUsage();
    }

    res.port = params[1 + res.new_network];
    res.pwd = params[2 + res.new_network];

    if (n_params == 4) {

        res.new_network = 1;

        vector<string> v = ft_split(params[1], ":");
        if ( v.size() != 3 )
            throw eUsage();

        res.host = v[0];
        res.port_nwk = v[1];
        res.pwd_nwk = v[2];
    }

    return res;
}