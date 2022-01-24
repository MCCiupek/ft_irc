#include "headers.hpp"

int display_usage( void ) {
    
    cout << "Usage: ./ircserv [host:port_network:password_network] <port> <password>" << endl;
    return 1;
}

//	Errors list
map<int, string>	err;

void    define_errors( void )
{
	err[ERR_NOSUCHNICK] = " :No such nick";
	err[ERR_NOSUCHSERVER] = " :No such server";
	err[ERR_NOSUCHCHANNEL] = " :No such channel";
	err[ERR_CANNOTSENDTOCHAN] = " :Cannot send to channel";
	err[ERR_TOOMANYCHANNELS] = " :You have joined too many channels";
	err[ERR_TOOMANYTARGETS] = " :Duplicate recipients. No message delivered";
	err[ERR_NOORIGIN] = " :No origin specified";
	err[ERR_NORECIPIENT] = " :No recipient given";
	err[ERR_NOTEXTTOSEND] = " :No text to send";
	err[ERR_NOMOTD] = " :MOTD File is missing";
	err[ERR_NONICKNAMEGIVEN] = " :No nickname given";
	err[ERR_ERRONEUSNICKNAME] = " :Erroneous nickname";
	err[ERR_NICKNAMEINUSE] = " :Nickname is already in use.";
	err[ERR_NOTONCHANNEL] = " :You're not on that channel";
	err[ERR_USERONCHANNEL] = " :You're not on that channel";
	err[ERR_NOTREGISTERED] = " :is already on channel";
	err[ERR_NEEDMOREPARAMS] = " :Not enough parameters";
	err[ERR_ALREADYREGISTRED] = " :You may not reregister";
	err[ERR_KEYSET] = " :Channel key already set";
	err[ERR_CHANNELISFULL] = " :Cannot join channel (+l)";
	err[ERR_UNKNOWNMODE] = " :is unknown mode char to me";
	err[ERR_INVITEONLYCHAN] = " :Cannot join channel (+i)";
	err[ERR_BANNEDFROMCHAN] = " :Cannot join channel (+b)";
	err[ERR_BADCHANNELKEY] = " :Cannot join channel (+k)";
	err[ERR_BADCHANMASK] = " :The given channel mask was invalid";
	err[ERR_CHANOPRIVSNEEDED] = " :You're not channel operator";
	err[ERR_UMODEUNKNOWNFLAG] = " :Unknown MODE flag";
	err[ERR_USERSDONTMATCH] = " :Cant change mode for other users";
}

void    send_error( User u, int errn, string arg )
{
	ostringstream s;

	if (*(arg.end() - 1) == '\n')
		arg = arg.substr(0, arg.length()-1);
	s << ":mfirc " << errn << " * " << arg << err[errn] << "\r\n";

	string msg = s.str();

	if ( send(u.getFd(), &msg[0], msg.size(), 0) == -1 )
		throw eExc(strerror(errno));
}

// void    send_reply( int fd, string msg )
// {
// 	if ( send(fd, &msg[0], msg.size(), 0) == -1 )
// 		throw eExc(strerror(errno));
// }

void    send_reply( User u, int rpln, string reply )
{
	ostringstream s;

	s	<< ":mfirc "
		<< setfill('0') << setw(3) << rpln
		<< " " << u.getNick() << " " << reply << "\r\n";
	string msg = s.str();

	if ( send(u.getFd(), &msg[0], msg.size(), 0) == -1 )
		throw eExc(strerror(errno));
}

void		send_notice_channel(User &u, Channel *c, string notice)
{
	vector<User*>	members = c->getMembers();

	for (vector<User*>::iterator it = members.begin(); it != members.end(); it++)
		send_notice(u, *(*it), notice);
}

void	send_notice( User from, User to, string notice )
{
	ostringstream s;

	s << ":" << from.fci() << " " << notice << "\r\n";

	string msg = s.str();

	if ( send(to.getFd(), &msg[0], msg.size(), 0) == -1 )
		throw eExc(strerror(errno));
}