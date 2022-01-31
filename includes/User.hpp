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
		string				_passwd;
		time_t 				*_last_act;
		bool				_ping_status;
		bool				_isset;			// If USER command is been used
		bool				_isIRCOper;		// If OPER command is been used
		bool				_isAuth;
		Channel				*_curr_chan;	// Last joined channel
		vector<Channel*>	_channels;		// Max chans 10
		

	public:

		/*								CONSTRUCTORS								*/

		User( void );
		User( int fd );
		User( int fd, string nick, string username, string hostname,
			string servername, string realname, string mode, bool ping_status );
		User( User const &src );
		~User( void );

		User				&operator=( User const &rhs );

		/*								GETTERS										*/

		int	const				&getFd( void ) const;
		string const			&getNick( void ) const;
		string const			&getUsername( void ) const;
		string const			&getHostname( void ) const;
		string const			&getServername( void ) const;
		string const			&getRealName( void ) const;
		string const			&getMode( void ) const;
		time_t 	*				getLastAct( void ) const;
		bool const				&getPingStatus( void ) const;
		bool const				&getIsSet( void ) const;
		bool const				&isAuth( void ) const;
		vector<Channel*>		getChans( void ) const;
		bool const				&getIsIRCOper( void ) const;
		Channel					*getCurrChan( void ) const;
		vector<Channel*> const	&getChannels( void ) const;

		/*								SETTERS										*/

		void					setFd( int fd );
		void					setNick( string nick );
		void 					setUsername( string username );
		void					setHostname( string hostname );
		void					setServername( string servername );
		void					setRealName( string realname );
		void					setMode( string mode );
		void					setPasswd( string passwd );
		void					setLastAct( time_t last_act );
		void					setPingStatus( bool ping_status );
		void					setIsSet( bool isset );
		void					setIsAuth( bool isauth );
		void					setIsIRCOper( bool isIRCOper );
		void					setCurrChan( Channel *c );

		/*								MEMBERS FUNCTIONS							*/


		bool					isRegistered( void ) const;
		bool					isIRCOper( void ) const;
		bool					isChanOper( void ) const;
		bool 					isVisible( void ) const;
		string const			fci( void ) const;
		string					addMode( string mode );
		string					rmMode( string mode );
		void					addChannel( Channel * channel );
		void					deleteChannel( Channel * channel );
		void					leaveAllChans( void );
		bool					isRegisteredToChan( Channel &c );
};

#endif