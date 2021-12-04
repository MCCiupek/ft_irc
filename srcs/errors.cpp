#include "headers.hpp"

int display_usage( void ) {
    
    cout << "Usage: ./ircserv [host:port_network:password_network] <port> <password>" << endl;
    return 1;
}

//	Errors list
map<int, string>	err;

void    define_errors( void )
{
	err[ERR_NONICKNAMEGIVEN] = " :No nickname given";
	err[ERR_ERRONEUSNICKNAME] = " :Erroneous nickname";
	err[ERR_NEEDMOREPARAMS] = " :Not enough parameters";
	err[ERR_NICKNAMEINUSE] = " :Nickname is already in use";
	err[ERR_ALREADYREGISTRED] = " :You may not reregister";
}

void    send_error( User u, int errn, string arg )
{
    string  buf = ":irc.local ";

	buf += to_string(errn);
	buf += " * ";
	buf += arg;
	buf += err[errn];
	buf += "\n";

	char *str = &buf[0];

	if ( send(u.getFd(), str, buf.size(), 0) == -1 )
				throw eExc(strerror(errno));
}