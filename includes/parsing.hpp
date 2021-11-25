#ifndef PARSING_HPP
# define PARSING_HPP

# include "headers.hpp"

struct parser_res {
        string     port;
        string  pwd;
        int     new_network;
        string  host;
        string     port_nwk;
        string  pwd_nwk;
};

parser_res parser( int n_params, char *params[] );

#endif
