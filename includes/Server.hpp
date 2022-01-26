#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"

// ************************************************************************** //
//                            	Server Class                                  //
// ************************************************************************** //

//class Channel;

class Server {

	private:

		/*								MEMBERS VARIABLES							*/

		string					_name;
		int						_status;
		int						_sockfd;
		int						_newfd;
		int						_fd_count;
		string					_port;
		string					_pwd;
		string					_host;
		struct addrinfo 		_hints;
		struct addrinfo			*_servinfo;
		struct pollfd			_poll[MAXCLI];
		map<int, User>			_users;
		map<int, string>		_usr_buf;
		vector<Channel*>		_channels;
		map<string, string>		_irc_operators;
		string					_motd;
		string					_creation_date;

		/*								CONSTRUCTORS								*/

		Server(Server const& src);

		/*								MEMBERS FUNCTIONS							*/

		void					setServinfo( void );
		int						setSocket( struct addrinfo * p );
		int						bindPort( struct addrinfo * p );
		void					listenHost( void );
		int						sendData( int fd );
		int						receiveData( int i );
		void					acceptConn( void );
		bool					add_to_pfds(int newfd);

	public:

		/*								CONSTRUCTORS								*/

		Server( void );
		Server(string port, string pwd);
		Server(string port, string pwd, string host, string motd, string operators);
		virtual ~Server( void ); 

		Server & operator=(Server const& src); 

		/*								GETTERS										*/

		string const				&getName( void ) const;
		string const 				&getPort( void ) const;
		string const 				&getPassword( void ) const;
		string const 				&getHost( void ) const;
		map<int, User> const		&getUsers( void ) const;
		vector<Channel*> const		&getChannels( void ) const;
		vector<string> const		getChannelsNames( void ) const;
		string const				&getMotd( void ) const;
		string const				&getCreationDate( void ) const;
		map<string, string>	const	&getIRCOperators( void ) const;

		/*								MEMBERS FUNCTIONS							*/

		void					initConn( void );
		void					run( void );
		bool					is_registered( User usr );
		bool					username_isIRCOper( string usr_name );
		bool					isIRCOperator( string usr_name, string pswd );
		Channel *				getChannelByName( string channel );
		Channel *				getChannelByKey( string key );
		User *					getUserByNick( string nick );
		void					addChannel( Channel * channel );
		void					deleteChannel( Channel * channel );
		void					deleteUser( User * u );
		void					del_from_pfds(int fd);

};

ostream & operator<<(ostream & stream, Server &Server);

#endif
