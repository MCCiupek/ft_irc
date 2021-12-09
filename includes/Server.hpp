#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"

// ************************************************************************** //
//                            	Server Class                                  //
// ************************************************************************** //

class Channel;

class Server {

	private:

		/*								MEMBERS VARIABLES							*/

		int						_status;
		int						_sockfd;
		int						_newfd;
		int						_fd_count;
		string					_port;
		string					_pwd;
		string					_host;
		string					_port_nwk;
		string					_pwd_nwk;
		struct addrinfo 		_hints;
		struct addrinfo			*_servinfo;
		struct pollfd			*_poll;
		map<int, User>			_users;
		vector<Channel*>		_channels;

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

	public:

		/*								CONSTRUCTORS								*/

		Server( void );
		Server(string port, string pwd);
		Server(string port, string pwd, string host, string port_nwk, string pwd_nwk); 
		virtual ~Server( void ); 

		Server & operator=(Server const& src); 

		/*								GETTERS										*/

		string const 			&getPort( void ) const;
		string const 			&getPassword( void ) const;
		string const 			&getHost( void ) const;
		string const 			&getPortNetwork( void ) const;
		string const 			&getPasswordNetwork( void ) const;
		map<int, User>			&getUsers( void );

		/*								MEMBERS FUNCTIONS							*/

		void					initConn( void );
		void					run( void );
		int						is_registered( User usr );
		Channel *				getChannelByName( string channel );
		Channel *				getChannelByKey( string key );

};

ostream & operator<<(ostream & stream, Server &Server);

#endif
