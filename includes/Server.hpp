#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"

// ************************************************************************** //
//                            	Server Class                                  //
// ************************************************************************** //

class Server {

	public:

		Server();
		Server(string port, string pwd);
		Server(string port, string pwd, string host, string port_nwk, string pwd_nwk); 
		virtual ~Server(); 

		Server & operator=(Server const& src); 

		string const &		getPort() const;
		string const &		getPassword() const;
		string const &		getHost() const;
		string const &		getPortNetwork() const;
		string const &		getPasswordNetwork() const;

		void				initConn();
		void				run();

		void				setServinfo();
		int					setSocket( struct addrinfo * p );
		int					bindPort( struct addrinfo * p );
		void				listenHost();
		int					receiveData( int i );
		int					sendData( int i );
		void				acceptConn();

	private:

		Server(Server const& src);

		int						_status;
		int						_sockfd;
		int						_newfd;
		int						_fd_count;
		char *					_s;
		string					_port;
		string					_pwd;
		string					_host;
		string					_port_nwk;
		string					_pwd_nwk;
		struct addrinfo 		_hints;
		struct addrinfo *		_servinfo;
		struct sockaddr_storage	_host_addr;
		struct pollfd *			_poll;
};

ostream & operator<<(ostream & stream, Server &Server);

#endif
