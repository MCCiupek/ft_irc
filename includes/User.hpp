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

	public:

		/*								CONSTRUCTORS								*/

		User( void ) : _fd(-1), _nick(""), _username(""), _hostname(""),
			_servername(""), _realname("")
		{
		}

		User( int fd ) : _fd(fd), _nick("Client #" + to_string(fd)), _username(""), _hostname(""),
			_servername(""), _realname("")
		{
		}

		User( int fd, string nick, string username, string hostname,
			string servername, string realname ) : _fd(fd), _nick(nick),
			_username(username), _hostname(hostname), _servername(servername),
			_realname(realname)
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

		/*								MEMBERS FUNCTIONS							*/
};

#endif