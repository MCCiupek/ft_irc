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
void		send_to_all_in_chan( Channel * Chan, string txt, User &usr );
void		privmsg( vector<string> args, User &usr, Server &srv );
void		notice( vector<string> args, User &usr, Server &srv );
void		part( vector<string> args, User &usr, Server &srv );
void		pass( vector<string> args, User &usr, Server &srv );
void		topic( vector<string> args, User &usr, Server &srv );
void		names( vector<string> args, User &usr, Server &srv );
void		quit( vector<string> args, User &usr, Server &srv );
void		kick( vector<string> args, User &usr, Server &srv );
void		invite( vector<string> args, User &usr, Server &srv );
void		oper( vector<string> args, User &usr, Server &srv );

bool		check_password( User &usr, Server &srv );

#endif
