#ifndef CMD_HPP
# define CMD_HPP

# include "headers.hpp"

void		nick( vector<string> args, User &usr, Server &srv );
void		user( vector<string> args, User &usr, Server &srv );
void		mode( vector<string> args, User &usr, Server &srv );
void		ping( vector<string> args, User &usr, Server &srv );
void		pong( vector<string> args, User &usr, Server &srv );
void		who( vector<string> args, User &usr, Server &srv );
void		join( vector<string> args, User &usr, Server &srv );
void		mode( vector<string> args, User &usr, Server &srv );
//void		cnl_mode( vector<string> args, User &usr, Server &srv );
//void		usr_mode( vector<string> args, User &usr, Server &srv );
void		send_to_all_in_chan( Channel * Chan, string txt, User &usr );
void		privmsg( vector<string> args, User &usr, Server &srv );
void		part( vector<string> args, User &usr, Server &srv );
void		topic( vector<string> args, User &usr, Server &srv );
void		names( vector<string> args, User &usr, Server &srv );

#endif
