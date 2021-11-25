#ifndef PARSING_HPP
# define PARSING_HPP

# include "headers.hpp"

struct parser_res {
        int     port;
        string  pwd;
        int     new_network;
        string  host;
        int     port_nwk;
        string  pwd_nwk;
};

parser_res parser( int n_params, char *params[] );

#endif
