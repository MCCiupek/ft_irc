#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "headers.hpp"

// OFFICIAL REPLIES

# define RPL_WELCOME(nick, user, host) (":Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host)
# define RPL_YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version)
# define RPL_CREATED(date) (":This server was created " + date)
# define RPL_MYINFO(servername, version, available_user_modes, available_channel_modes) \
	(":" + servername + " " + version + " " + available_user_modes + " " + available_channel_modes)
# define RPL_MOTDSTART(server) (":- " + server + " Message of the day -")
# define RPL_MOTD(txt) (txt)
# define RPL_ENDOFMOTD() (":End of /MOTD command")
# define RPL_UMODEIS(user_mode) (user_mode)
# define RPL_WHOREPLY(arg) (arg)
# define RPL_ENDOFWHO(name) (name + " :End of /WHO list")
# define RPL_CHANNELMODEIS(channel, mode) (" :" + channel + " " + mode)
# define RPL_NAMREPLY(channel, list) (" = #" + channel + ":" + list)
# define RPL_ENDOFNAMES(channel) (" :" + channel + " :End of NAMES list")
# define RPL_AWAY(nick, msg) (nick + " :" + msg)

// COMMAND REPLYES
// irssi JOIN [ :<nickname>!<nickname>@<host> JOIN :#<channel> ]
// # define RPL_JOIN(fci, channel) (":" + fci + " JOIN " + ":#" + channel)

// ERRORS

# define ERR_NOSUCHNICK			401
# define ERR_NOSUCHSERVER		402
# define ERR_NOSUCHCHANNEL		403
# define ERR_CANNOTSENDTOCHAN	404
# define ERR_TOOMANYCHANNELS	405
# define ERR_WASNOSUCHNICK		406 // "<nickname> :There was no such nickname"
# define ERR_TOOMANYTARGETS		407
# define ERR_NOORIGIN			409
# define ERR_NORECIPIENT		411
# define ERR_NOTEXTTOSEND		412
# define ERR_NOTOPLEVEL			413 // "<mask> :No toplevel domain specified"
# define ERR_WILDTOPLEVEL		414 // "<mask> :Wildcard in toplevel domain"
# define ERR_UNKNOWNCOMMAND		421 // "<command> :Unknown command"
# define ERR_NOMOTD				422
# define ERR_NOADMININFO		423 // "<server> :No administrative info available"
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_NOTONCHANNEL		442
# define ERR_NOTREGISTERED		451
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462
# define ERR_KEYSET				467
# define ERR_CHANNELISFULL		471
# define ERR_UNKNOWNMODE		472
# define ERR_INVITEONLYCHAN		473
# define ERR_BANNEDFROMCHAN		474
# define ERR_BADCHANNELKEY		475
# define ERR_BADCHANMASK		476
# define ERR_CHANOPRIVSNEEDED	482
# define ERR_UMODEUNKNOWNFLAG	501
# define ERR_USERSDONTMATCH		502


class eExc : public exception {
	
	public:

		eExc(const char * str) : _strerror(str) {};
		const char * what() const throw() { return _strerror; };

	private:

		const char * _strerror;
};

extern map<int, string>		err;	// errors list

int		display_usage( void );
void    define_errors( void );
void    send_error( User u, int errn, string cmd );
void    send_reply( User u, int rpln, string reply );

#endif
