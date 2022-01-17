#ifndef USER_HPP
# define USER_HPP

# include "headers.hpp"
# include "Channel.hpp"

// ************************************************************************** //
//                            	 User Class                                   //
// ************************************************************************** //

class Channel;

class User
{
	private:

		int					_fd;
		string				_nick;
		string				_username;
		string				_hostname;
		string				_servername;
		string				_realname;
		string				_mode;
		time_t 				*_last_act;
		bool				_ping_status;
		bool				_is_away;
		bool				_isset;			// If USER command is been used
		bool				_isIRCOper;		// If OPER command is been used
		Channel				*_curr_chan;	// Last joined channel
		vector<Channel*>	_channels;		// Max chans 10
		

	public:

		/*								CONSTRUCTORS								*/

		User( void ) : _fd(-1), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname(""), _mode(""), _ping_status(false),
			_is_away(false), _isset(false), _isIRCOper(false), _curr_chan(nullptr),
			_channels()
		{
		}

		User( int fd ) : _fd(fd), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname(""), _mode(""), _ping_status(false),
			_is_away(false), _isset(false),  _isIRCOper(false), _curr_chan(nullptr),
			_channels()
		{
		}

		User( int fd, string nick, string username, string hostname,
			string servername, string realname, string mode, bool ping_status ) : _fd(fd), _nick(nick),
			_username(username), _hostname(hostname), _servername(servername),
			_realname(realname), _mode(mode), _ping_status(ping_status),
			_is_away(false), _isset(false),  _isIRCOper(false), _curr_chan(nullptr),
			_channels()
		{
		}

		User( User const &src )
		{
			*this = src;
		}

		~User( void )
		{
		}

		User				&operator=( User const &rhs )
		{
			_fd = rhs._fd;
			_nick = rhs._nick;
			_username = rhs._username;
			_hostname = rhs._hostname;
			_servername = rhs._servername;
			_realname = rhs._realname;
			_mode = rhs._mode;
			_last_act = rhs._last_act;
			_ping_status = rhs._ping_status;
			_is_away = rhs._is_away;
			_isset = rhs._isset;
			_isIRCOper = rhs._isIRCOper;
			_curr_chan = rhs._curr_chan;
			_channels = rhs._channels;
		
			return (*this);
		}

		/*								GETTERS										*/

		int	const			&getFd( void ) const
		{
			return _fd;
		}

		string const		&getNick( void ) const
		{
			return _nick;
		}

		string const		&getUsername( void ) const
		{
			return _username;
		}

		string const		&getHostname( void ) const
		{
			return _hostname;
		}

		string const		&getServername( void ) const
		{
			return _servername;
		}

		string const		&getRealName( void ) const
		{
			return _realname;
		}

		string const		&getMode( void ) const
		{
			return _mode;
		}

		time_t 	*			getLastAct( void ) const
		{
			return _last_act;
		}

		bool const			&getPingStatus( void ) const
		{
			return _ping_status;
		}

		bool const			&getIsAway( void ) const
		{
			return _is_away;
		}
		
		bool const			&getIsSet( void ) const
		{
			return _isset;
		}

		bool const			&getIsIRCOper( void ) const
		{
			return _isIRCOper;
		}

		Channel				*getCurrChan( void ) const
		{
			return _curr_chan;
		}

		vector<Channel*> const		&getChannels( void ) const
		{
			return _channels;
		}

		/*								SETTERS										*/

		void				setFd( int fd )
		{
			_fd = fd;
		}

		void				setNick( string nick )
		{
			_nick = nick;
		}

		void 				setUsername( string username )
		{
			_username = username;
		}

		void				setHostname( string hostname )
		{
			_hostname = hostname;
		}

		void				setServername( string servername )
		{
			_servername = servername;
		}

		void				setRealName( string realname )
		{
			_realname = realname;
		}

		void				setMode( string mode )
		{
			_mode = mode;
		}

		void				setLastAct( time_t last_act )
		{
			_last_act = &last_act;
		}

		void				setPingStatus( bool ping_status )
		{
			_ping_status = ping_status;
		}

		void				setIsAway( bool is_away )
		{
			_is_away = is_away;
		}


		void				setIsSet( bool isset )
		{
			_isset = isset;
		}

		void				setIsIRCOper( bool isIRCOper )
		{
			_isIRCOper = isIRCOper;
		}

		void				setCurrChan( Channel *c )
		{
			_curr_chan = c;
			cout << MAGENTA << getNick() << "'s current channel set to " << getCurrChan() << RESET << endl;
		}

		/*								MEMBERS FUNCTIONS							*/

		bool				isRegistered( void ) const
		{
			if (!getNick().empty() && getIsSet())
				return true;
			
			return false;
		}

		bool				isIRCOper( void ) const
		{
			return _mode.find('o') == string::npos;
		}

		bool				isChanOper( void ) const
		{
			for (vector<Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
				if ((*it)->getOperator()->getNick() == _nick)
					return true;
			
			return false;
		}

		bool 				isVisible( void ) const
		{
			return _mode.find('i') == string::npos;
		}

		string const		fci( void ) const
		{
			// Returns Full-Client Identifier (FCI): <nick>!<user>@<host>
			return _nick + "!" + _username + "@" + _hostname;
		}

		void				addChannel( Channel * channel ) {
			
			if (_channels.size() < 10)
			{
				cout << MAGENTA << this->getNick() << " joined channel " << channel->getName() << RESET << endl;
				_channels.push_back(channel);
			}
			else
				cout << MAGENTA << getNick() << " max number of channels reached." << RESET << endl;

		}

		void				leaveChannel( Channel * channel ) {
			
			cout << MAGENTA << this->getNick() << " left channel " << channel->getName() << RESET << endl;
			channel->deleteMember(this);
			for ( vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++ ) {
				if ( (*it)->getName() == channel->getName() )
					_channels.erase(it);
				// TO DO: transfert channel ownership ?
			}
		}

		void				leaveAllChans( void ) {
			
			while ( !(_channels.empty()) ) {
				(*_channels.begin())->deleteMember(this);
				cout << MAGENTA << this->getNick() << " left channel " << (*_channels.begin())->getName() << RESET << endl;
				_channels.erase(_channels.begin());
				// TO DO: transfert channel ownership ?
			}

		}

		bool				isRegisteredToChan( Channel &c )
		{
			cout << "isregistered to chan" << endl;
			cout << _channels.size() << endl;

			for (vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it )
			{
				if ((*it)->getName() == c.getName())
					return true;
			}

			return false;
		}

};

#endif