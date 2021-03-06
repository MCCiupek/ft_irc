#ifndef PARSING_HPP
# define PARSING_HPP

# include "headers.hpp"
# include "Server.hpp"

map<string, string> parser( int n_params, char *params[] );
int 				parsing( vector<string> args, User &usr, Server &srv );
map<string, string>	conf_file( char *path );

typedef void (*FnPtr)(vector<string>, User&, Server&);

#endif
