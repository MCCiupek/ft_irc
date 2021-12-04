#ifndef PARSING_HPP
# define PARSING_HPP

# include "headers.hpp"
# include "Server.hpp"

map<string, string> parser( int n_params, char *params[] );
void				parsing( vector<string> args, User &usr, Server &srv );

typedef void (*FnPtr)(vector<string>, User&, Server&);

#endif
