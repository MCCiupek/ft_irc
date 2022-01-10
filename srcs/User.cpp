#include "headers.hpp"

User::User( void ) : _fd(-1), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname(""), _mode(""), _away_msg("is away"), _ping_status(false),
			_is_away(false), _isset(false)
{
}

User::User( int fd ) : _fd(fd), _nick(""), _username(""), _hostname(""),
	_servername(""), _realname(""), _mode(""), _away_msg("is away"), _ping_status(false),
	_is_away(false), _isset(false)
{
}

User::User( int fd, string nick, string username, string hostname,
	string servername, string realname, string mode, string away_msg, bool ping_status ) : _fd(fd), _nick(nick),
	_username(username), _hostname(hostname), _servername(servername),
	_realname(realname), _mode(mode), _away_msg(away_msg), _ping_status(ping_status),
	_is_away(false), _isset(false)
{
}

User::User( User const &src )
{
	*this = src;
}

User::~User( void )
{
}

User				&User::operator=( User const &rhs )
{
	_fd = rhs._fd;
	_nick = rhs._nick;
	_username = rhs._username;
	_hostname = rhs._hostname;
	_servername = rhs._servername;
	_realname = rhs._realname;

	return (*this);
}

/*								GETTERS										*/

int	const			&User::getFd( void ) const
{
	return _fd;
}

string const		&User::getNick( void ) const
{
	return _nick;
}

string const		&User::getUsername( void ) const
{
	return _username;
}

string const		&User::getHostname( void ) const
{
	return _hostname;
}

string const		&User::getServername( void ) const
{
	return _servername;
}

string const		&User::getRealName( void ) const
{
	return _realname;
}

string const		&User::getMode( void ) const
{
	return _mode;
}

string const		&User::getAwayMsg( void ) const
{
	return _away_msg;
}

time_t 	*			User::getLastAct( void ) const
{
	return _last_act;
}

bool const			&User::getPingStatus( void ) const
{
	return _ping_status;
}

bool const			&User::getIsAway( void ) const
{
	return _is_away;
}

bool const			&User::getIsSet( void ) const
{
	return _isset;
}

vector<Channel*>	User::getChans( void ) const
{
	return _channels;
}

/*								SETTERS										*/

void				User::setFd( int fd )
{
	_fd = fd;
}

void				User::setNick( string nick )
{
	_nick = nick;
}

void 				User::setUsername( string username )
{
	_username = username;
}

void				User::setHostname( string hostname )
{
	_hostname = hostname;
}

void				User::setServername( string servername )
{
	_servername = servername;
}

void				User::setRealName( string realname )
{
	_realname = realname;
}

void				User::setMode( string mode )
{
	_mode = mode;
}

void				User::setLastAct( time_t last_act )
{
	_last_act = &last_act;
}

void				User::setPingStatus( bool ping_status )
{
	_ping_status = ping_status;
}

void				User::setAway( string msg="" )
{
	_is_away = true;
	if ( msg != "" )
		_away_msg = msg;
}

void				User::unsetAway( void )
{
	_is_away = false;
}

void				User::setIsSet( bool isset )
{
	_isset = isset;
}

/*								MEMBERS FUNCTIONS							*/

bool				User::isOper( void ) const
{
	return _mode.find('o') == string::npos;
}

bool 				User::isVisible( void ) const
{
	return _mode.find('i') == string::npos;
}

string const		User::fci( void ) const
{
	// Returns Full-Client Identifier (FCI): <nick>!<user>@<host>
	return _nick + "!" + _username + "@" + _hostname;
}

void				User::addChannel( Channel * channel ) {
	
	cout << MAGENTA << this->getNick() << " joined channel " << channel->getName() << RESET << endl;
	_channels.push_back(channel);
}

void				User::deleteChannel( Channel * channel ) {
	
	for ( vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++ ) {
		if ( (*it)->getName() == channel->getName() )
			_channels.erase(it);
			return ;
		// TO DO: transfert channel ownership ?
	}
}

void				User::leaveChannel( Channel * channel ) {
	
	cout << MAGENTA << this->getNick() << " left channel " << channel->getName() << RESET << endl;
	channel->deleteMember(this);
	deleteChannel(channel);
}

void				User::leaveAllChans( void ) {
	
	while ( !(_channels.empty()) ) {
		(*_channels.begin())->deleteMember(this);
		cout << MAGENTA << this->getNick() << " left channel " << (*_channels.begin())->getName() << RESET << endl;
		_channels.erase(_channels.begin());
		// TO DO: transfert channel ownership ?
	}

}
