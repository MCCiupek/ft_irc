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

		int			_fd;
		string		_nick;
		string		_username;
		string		_hostname;
		string		_servername;
		string		_realname;
		string		_mode;
		string		_away_msg;
		time_t *	_last_act;
		bool		_ping_status;
		bool		_is_away;
		bool		_isset;			// If USER command is been used
		vector<Channel*>	_channels;

	public:

		/*								CONSTRUCTORS								*/

		User( void ) : _fd(-1), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname(""), _mode(""), _away_msg("is away"), _ping_status(false),
			_is_away(false), _isset(false)
		{
		}

		User( int fd ) : _fd(fd), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname(""), _mode(""), _away_msg("is away"), _ping_status(false),
			_is_away(false), _isset(false)
		{
		}

		User( int fd, string nick, string username, string hostname,
			string servername, string realname, string mode, string away_msg, bool ping_status ) : _fd(fd), _nick(nick),
			_username(username), _hostname(hostname), _servername(servername),
			_realname(realname), _mode(mode), _away_msg(away_msg), _ping_status(ping_status),
			_is_away(false), _isset(false)
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

		string const		&getAwayMsg( void ) const
		{
			return _away_msg;
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

		vector<Channel*>	getChans( void ) const
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

		void				setAway( string msg="" )
		{
			_is_away = true;
			if ( msg != "" )
				_away_msg = msg;
		}

		void				unsetAway( void )
		{
			_is_away = false;
		}

		void				setIsSet( bool isset )
		{
			_isset = isset;
		}

		/*								MEMBERS FUNCTIONS							*/
		
		bool				isOper( void ) const
		{
			return _mode.find('o') == string::npos;
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
			
			cout << MAGENTA << this->getNick() << " joined channel " << channel->getName() << RESET << endl;
			_channels.push_back(channel);
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

};

#endif