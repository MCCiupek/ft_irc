#include "headers.hpp"

User::User( void ) : _fd(-1), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname(""), _mode(""), _passwd(""), 
			_ping_status(false), _isset(false), _isIRCOper(false), _isAuth(false),
			_curr_chan(NULL), _channels()
{
}

User::User( int fd ) : _fd(fd), _nick(""), _username(""), _hostname(""),
	_servername(""), _realname(""), _mode(""), _passwd(""), _ping_status(false),
	_isset(false),  _isIRCOper(false), _isAuth(false), _curr_chan(NULL), _channels()
{
}

User::User( int fd, string nick, string username, string hostname,
	string servername, string realname, string mode, bool ping_status ) :
	_fd(fd), _nick(nick), _username(username), _hostname(hostname), _servername(servername),
	_realname(realname), _mode(mode), _ping_status(ping_status), _isset(false),
	_isIRCOper(false), _isAuth(false), _curr_chan(NULL), _channels()
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
	_mode = rhs._mode;
	_passwd = rhs._passwd;
	_last_act = rhs._last_act;
	_ping_status = rhs._ping_status;
	_isset = rhs._isset;
	_isIRCOper = rhs._isIRCOper;
	_curr_chan = rhs._curr_chan;
	_channels = rhs._channels;

	return (*this);
}

/*								GETTERS										*/

int	const				&User::getFd( void ) const
{
	return _fd;
}

string const			&User::getNick( void ) const
{
	return _nick;
}

string const			&User::getUsername( void ) const
{
	return _username;
}

string const			&User::getHostname( void ) const
{
	return _hostname;
}

string const			&User::getServername( void ) const
{
	return _servername;
}

string const			&User::getRealName( void ) const
{
	return _realname;
}

string const			&User::getMode( void ) const
{
	return _mode;
}

time_t 	*				User::getLastAct( void ) const
{
	return _last_act;
}

bool const				&User::getPingStatus( void ) const
{
	return _ping_status;
}

bool const				&User::getIsSet( void ) const
{
	return _isset;
}

bool const				&User::isAuth( void ) const
{
	return _isAuth;
}

vector<Channel*>		User::getChans( void ) const
{
	return _channels;
}

bool const				&User::getIsIRCOper( void ) const
{
	return _isIRCOper;
}

Channel					*User::getCurrChan( void ) const
{
	return _curr_chan;
}

vector<Channel*> const	&User::getChannels( void ) const
{
	return _channels;
}

/*								SETTERS										*/

void					User::setFd( int fd )
{
	_fd = fd;
}

void					User::setNick( string nick )
{
	_nick = nick;
}

void 					User::setUsername( string username )
{
	_username = username;
}

void					User::setHostname( string hostname )
{
	_hostname = hostname;
}

void					User::setServername( string servername )
{
	_servername = servername;
}

void					User::setRealName( string realname )
{
	_realname = realname;
}

void					User::setMode( string mode )
{
	_mode = mode;
}

void					User::setPasswd( string passwd )
{
	_passwd = passwd;
}

void					User::setLastAct( time_t last_act )
{
	_last_act = &last_act;
}

void					User::setPingStatus( bool ping_status )
{
	_ping_status = ping_status;
}

void					User::setIsSet( bool isset )
{
	_isset = isset;
}

void					User::setIsAuth( bool isauth )
{
	_isAuth = isauth;
}


void					User::setIsIRCOper( bool isIRCOper )
{
	_isIRCOper = isIRCOper;
	if (isIRCOper)
		addMode("o");
	else
		rmMode("o");
}

void					User::setCurrChan( Channel *c )
{
	_curr_chan = c;
	if (_curr_chan)
		cout << MAGENTA << getNick() << "'s current channel set to " << getCurrChan()->getName() << RESET << endl;
	else
		cout << MAGENTA << getNick() << " isnt on any channel" << RESET << endl;
}

/*								MEMBERS FUNCTIONS							*/

bool				User::isRegistered( void ) const
{
	if (!getNick().empty() && getIsSet())
		return true;
	
	return false;
}

bool				User::isIRCOper( void ) const
{
	return _isIRCOper;
}

bool				User::isChanOper( void ) const
{
	for (vector<Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
		if ((*it)->isOper(*this))
			return true;
	
	return false;
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

string				User::addMode( string mode )
{
	string	to_add = "";

	cout << "oper = " << isIRCOper() << endl;

	for (size_t i = 0; i < mode.size(); i++) {
		if (( _mode.find(mode[i]) == string::npos) && mode[i] != 'o')
			to_add += mode[i];
		else if (mode[i] == 'o' && isIRCOper()) // IRC operator
			to_add += mode[i];
	}

	_mode += to_add;

	return (to_add);
}

string				User::rmMode( string mode )
{
	string	removed = "";

	for (size_t i = 0; i < mode.size(); i++) {
		size_t to_remove = _mode.find(mode[i]);
		if ( to_remove != string::npos )
		{
			_mode.erase(_mode.begin() + to_remove);
			removed += mode[i];
		}
	}

	return (removed);
}

void				User::addChannel( Channel * channel ) {
	
	if (_channels.size() < MAX_CHAN_PER_USR)
	{
		cout << MAGENTA << this->getNick() << " joined channel " << channel->getName() << RESET << endl;
		_channels.push_back(channel);
	}
	else
		cout << MAGENTA << getNick() << " max number of channels reached." << RESET << endl;
}

void				User::deleteChannel( Channel * channel ) {

	string chan_name;

	channel->deleteOper(this);
	channel->deleteMember(this);
	for ( vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++ ) {
		if ( (*it)->getName() == channel->getName() ) {
			if ( this->getCurrChan() && this->getCurrChan()->getName() == channel->getName() ) {
				if ( this->getChannels().size() > 1 ) {
					if (this->getChannels().back()->getName() != channel->getName())
						this->setCurrChan(this->getChannels().back());
					else
						this->setCurrChan(this->getChannels().front());
				}
				else
					this->setCurrChan(NULL);
			}
			cout << MAGENTA << this->getNick() << " left channel " << channel->getName() << RESET << endl;
			_channels.erase(it);
			return ;
		}
	}
}

void				User::leaveAllChans( void ) {
	
	while ( !(_channels.empty()) ) {
		
		vector<Channel*>::iterator	chan = _channels.begin();
		deleteChannel( *chan );
	}

}

bool				User::isRegisteredToChan( Channel &c )
{
	// cout << "isregistered to chan" << endl;
	// cout << _channels.size() << endl;

	for (vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it )
	{
		if ((*it)->getName() == c.getName())
			return true;
	}

	return false;
}
