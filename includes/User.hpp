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

		User( void );
		User( int fd );
		User( int fd, string nick, string username, string hostname,
			string servername, string realname, string mode, string away_msg, bool ping_status );
		User( User const &src );
		~User( void );
		User				&operator=( User const &rhs );

		/*								GETTERS										*/

		int	const			&getFd( void ) const;
		string const		&getNick( void ) const;
		string const		&getUsername( void ) const;
		string const		&getHostname( void ) const;
		string const		&getServername( void ) const;
		string const		&getRealName( void ) const;
		string const		&getMode( void ) const;
		string const		&getAwayMsg( void ) const;
		time_t 	*			getLastAct( void ) const;
		bool const			&getPingStatus( void ) const;
		bool const			&getIsAway( void ) const;
		bool const			&getIsSet( void ) const;
		vector<Channel*>	getChans( void ) const;

		/*								SETTERS										*/

		void				setFd( int fd );
		void				setNick( string nick );
		void 				setUsername( string username );
		void				setHostname( string hostname );
		void				setServername( string servername );
		void				setRealName( string realname );
		void				setMode( string mode );
		void				setLastAct( time_t last_act );
		void				setPingStatus( bool ping_status );
		void				setAway( string msg );
		void				unsetAway( void );
		void				setIsSet( bool isset );

		/*								MEMBERS FUNCTIONS							*/
		
		bool				isOper( void ) const;
		bool 				isVisible( void ) const;
		string const		fci( void ) const;
		void				addChannel( Channel * channel );
		void				deleteChannel( Channel * channel );
		void				leaveChannel( Channel * channel );
		void				leaveAllChans( void );

};

#endif