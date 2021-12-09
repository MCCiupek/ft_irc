#ifndef USER_HPP
# define USER_HPP

# include "headers.hpp"

// ************************************************************************** //
//                            	 User Class                                   //
// ************************************************************************** //

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
		time_t *	_last_act;
		bool		_ping_status;
		bool		_is_away;

	public:

		/*								CONSTRUCTORS								*/

		User( void ) : _fd(-1), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname(""), _mode(""), _ping_status(false),
			_is_away(false)
		{
		}

		User( int fd ) : _fd(fd), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname(""), _mode(""), _ping_status(false),
			_is_away(false)
		{
		}

		User( int fd, string nick, string username, string hostname,
			string servername, string realname, string mode, bool ping_status ) : _fd(fd), _nick(nick),
			_username(username), _hostname(hostname), _servername(servername),
			_realname(realname), _mode(mode), _ping_status(ping_status),
			_is_away(false)
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
};

#endif