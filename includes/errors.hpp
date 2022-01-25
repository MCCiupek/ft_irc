#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "headers.hpp"

// OFFICIAL REPLIES

# define RPL_WELCOME(nick, user, host) (":Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
# define RPL_YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version + "\r\n")
# define RPL_CREATED(date) (":This server was created " + date + "\r\n")
# define RPL_MYINFO(servername, version, available_user_modes, available_channel_modes) \
	(":" + servername + " " + version + " " + available_user_modes + " " + available_channel_modes + "\r\n")
# define RPL_MOTDSTART(server) (":- " + server + " Message of the day - \r\n")
# define RPL_MOTD(txt) (txt + "\r\n")
# define RPL_ENDOFMOTD() (":End of /MOTD command\r\n")
# define RPL_UMODEIS(user_mode) (user_mode + "\r\n")
# define RPL_WHOREPLY(curr_chan, username, hostname, servname, nick, ircoper, chanoper, realname) \
	(curr_chan + " " + username + " " + hostname + " " + servname + " " + nick + " H" + ircoper \
	+ chanoper + " :0 " + realname)
# define RPL_ENDOFWHO(name) (name + " :End of /WHO list\r\n")
# define RPL_CHANNELMODEIS(channel, mode) (channel + " :+" + mode + "\r\n")
# define RPL_NOTOPIC(channel) (channel + " :No topic is set" + "\r\n")
# define RPL_TOPIC(channel, topic) (channel + " :" + topic + "\r\n")
# define RPL_TOPICWHOTIME(channel, who, when) (channel + " " + who + " :" + when + "\r\n")
# define RPL_NAMREPLY(channel, list) ("= " + channel + " :" + list + "\r\n")
# define RPL_ENDOFNAMES(channel) (channel + " :End of /NAMES list.\r\n")
# define RPL_AWAY(nick, msg) (nick + " :" + msg + "\r\n")
# define RPL_CREATIONTIME(channel, creation_time) (channel + " :" + creation_time + "\r\n")
# define RPL_BANLIST(channel, mask) (channel + " :" + mask + "\r\n")
# define RPL_ENDOFBANLIST(channel) (channel + " :End of channel ban list\r\n")
# define RPL_INVITING(guest, channel) (guest + " :" + channel + "\r\n")


// NOTICES

# define NTC_NICK(nick) ("NICK :" + nick)
# define NTC_MODE(nick, mode) ("MODE " + nick + " :" + mode)
# define NTC_JOIN(channel) ("JOIN :" + channel)
# define NTC_PART(channel) ("PART :" + channel)
# define NTC_PART_MSG(channel, msg) ("PART " + channel + " :\"" + msg +"\"")
# define NTC_PRIVMSG(dest, msg) ("PRIVMSG " + dest + " " + msg)
# define NTC_NOTICE(dest, msg) ("NOTICE " + dest + " " + msg)
# define NTC_QUIT(msg) (" QUIT :Quit: " + msg)
# define NTC_TOPIC(channel, topic) ("TOPIC " + channel + " :" + topic)
# define NTC_CHANMODE(channel, mode) ("MODE " + channel + " :" + mode)
# define NTC_CHANMODE_ARG(channel, mode, arg) ("MODE " + channel + " " + mode + " :" + arg)
# define NTC_KICK(channel, usr, reason) ("KICK " + channel  + " " + usr + " " + reason)
# define NTC_INVITE(channel, usr) ("INVITE " + usr  + " :" + channel)

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
# define ERR_USERONCHANNEL		443
# define ERR_NOTREGISTERED		451
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462
# define ERR_PASSWDMISMATCH		464
# define ERR_KEYSET				467
# define ERR_CHANNELISFULL		471
# define ERR_UNKNOWNMODE		472
# define ERR_INVITEONLYCHAN		473
# define ERR_BANNEDFROMCHAN		474
# define ERR_BADCHANNELKEY		475
# define ERR_BADCHANMASK		476
# define ERR_CHANOPRIVSNEEDED	482
# define ERR_NOOPERHOST			491
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
void	send_notice_channel(User &u, Channel *c, string notice);
void    send_notice( User from, User to, string notice );

#endif
