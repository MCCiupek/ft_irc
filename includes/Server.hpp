#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"

// ************************************************************************** //
//                            	Server Class                                  //
// ************************************************************************** //

class Server {

	public:

		Server();
		Server(int port, string pwd);
		Server(int port, string pwd, string host, int port_nwk, string pwd_nwk); 
        virtual ~Server(); 

		Server & operator=(Server const& src); 

		int						getPort() const;
		std::string const &		getPassword() const;
		std::string const &		getHost() const;
		int						getPortNetwork() const;
		std::string const &		getPasswordNetwork() const;

	private:

		Server(Server const& src);

		int		_port;
		string	_pwd;
		string	_host;
		int		_port_nwk;
		string	_pwd_nwk;
};

ostream & operator<<(ostream & stream, Server &Server);

#endif
