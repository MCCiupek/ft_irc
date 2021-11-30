#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"
# define BACKLOG 5

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

		void				setServinfo();
		int					setSocket( struct addrinfo * p );
		int					bindPort( struct addrinfo * p );
		void				listenHost();
		void				setSA();
		void				acceptConn();

	private:

		Server(Server const& src);

		int						_status;
		int						_sockfd;
		int						_newfd;
		char *					_s;
		string					_port;
		string					_pwd;
		string					_host;
		string					_port_nwk;
		string					_pwd_nwk;
		struct addrinfo 		_hints;
		struct addrinfo *		_servinfo;
		struct sockaddr_storage	_host_addr;
		struct sigaction 		_sa;
};

ostream & operator<<(ostream & stream, Server &Server);

#endif
