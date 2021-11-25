#include "headers.hpp"

Server::Server( void ) {}

Server::Server(int port, string pwd) : 
        _port(port), 
        _pwd(pwd),
        _host("new_network"),
        _port_nwk(0),
        _pwd_nwk("")
{

}

Server::Server(int port, string pwd, string host="new_network", int port_nwk=0, string pwd_nwk="") : 
        _port(port), 
        _pwd(pwd),
        _host(host),
        _port_nwk(port_nwk),
        _pwd_nwk(pwd_nwk)
{

}

Server::~Server() {

}

Server & Server::operator=(Server const & src) {

    if (this != &src) {
		this->_port = src.getPort();
        this->_pwd = src.getPassword();
        this->_host = src.getHost();
        this->_port_nwk = src.getPortNetwork();
        this->_pwd_nwk = src.getPasswordNetwork();
    }
	return *this;
}

int Server::getPort() const {
	return _port;
}

string const & Server::getPassword() const {
	return _pwd;
}

string const & Server::getHost() const {
	return _host;
}

int Server::getPortNetwork() const {
	return _port_nwk;
}

string const & Server::getPasswordNetwork() const {
	return _pwd_nwk;
}

ostream & operator<<(ostream & stream, Server &Server) {

    stream << "port: " << Server.getPort() << endl;
    stream << "pwd: " << Server.getPassword() << endl;
    stream << "host: " << Server.getHost() << endl;
    stream << "port network: " << Server.getPortNetwork() << endl;
    stream << "pwd network: " << Server.getPasswordNetwork() << endl;
    return stream;
}
