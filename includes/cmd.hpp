#ifndef CMD_HPP
# define CMD_HPP

# include "headers.hpp"

void		nick( vector<string> args, User &usr, Server &srv );
void		user( vector<string> args, User &usr, Server &srv );
void		ping( vector<string> args, User &usr, Server &srv );
void		pong( vector<string> args, User &usr, Server &srv );
void		who( vector<string> args, User &usr, Server &srv );

#endif
