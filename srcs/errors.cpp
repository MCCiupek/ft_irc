#include "headers.hpp"

int display_usage( void ) {
    
    cout << "Usage: ./ircserv [host:port_network:password_network] <port> <password>" << endl;
    return 1;
}

//	Errors list
map<int, string>	err;

void    define_errors( void )
{
	err[ERR_NOSUCHNICK] = " :No such nick/channel";
	err[ERR_NOSUCHSERVER] = " :No such server";
	err[ERR_NOORIGIN] = ":No origin specified";
	err[ERR_NORECIPIENT] = " :No recipient given";
	err[ERR_NOTEXTTOSEND] = " :No text to send";
	err[ERR_NONICKNAMEGIVEN] = " :No nickname given";
	err[ERR_ERRONEUSNICKNAME] = " :Erroneous nickname";
	err[ERR_NICKNAMEINUSE] = " :Nickname is already in use";
	err[ERR_NEEDMOREPARAMS] = " :Not enough parameters";
	err[ERR_ALREADYREGISTRED] = " :You may not reregister";
	err[ERR_UNKNOWNMODE] = " :is unknown mode char to me";
	err[ERR_USERSDONTMATCH] = " :Cant change mode for other users";
}

void    send_error( User u, int errn, string arg )
{
	ostringstream s;
    //string  buf = ":irc.local ";

	s << ":irc.local " << errn << " * " << arg << err[errn] << "\r\n";
	// buf += itoa(errn);//.c_str();
	// buf += " * ";
	// buf += arg;
	// buf += err[errn];
	// buf += "\r\n";

	//char *str = &buf[0];
	string msg = s.str();

	if ( send(u.getFd(), &msg[0], msg.size(), 0) == -1 )
		throw eExc(strerror(errno));
}

void    send_reply( User u, int rpln, string reply )
{
	ostringstream s;
    //string  buf = ":irc.local ";

	s << ":irc.local " << rpln << " " << u.getNick() << " " << reply;
	// buf += itoa(rpln);//.c_str();
	// buf += " ";
	// buf += u.getNick();
	// buf += " ";
	// buf += reply;

	string msg = s.str();

	if ( send(u.getFd(), &msg[0], msg.size(), 0) == -1 )
		throw eExc(strerror(errno));
}