#include "headers.hpp"

Server::Server( void ) {

}

Server::Server(string port, string pwd) : 
        _port(port), 
        _pwd(pwd),
        _host("localhost"),
        _port_nwk("0"),
        _pwd_nwk("")
{
    this->setServinfo();
    this->setSocket();
}

Server::Server(string port, string pwd, string host="localhost", string port_nwk="0", string pwd_nwk="") : 
        _port(port), 
        _pwd(pwd),
        _host(host),
        _port_nwk(port_nwk),
        _pwd_nwk(pwd_nwk)
{
    this->setServinfo();
    this->setSocket();
}

Server::~Server() {
    
    freeaddrinfo(_servinfo);
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

string const & Server::getPort() const {
	return _port;
}

string const & Server::getPassword() const {
	return _pwd;
}

string const & Server::getHost() const {
	return _host;
}

string const & Server::getPortNetwork() const {
	return _port_nwk;
}

string const & Server::getPasswordNetwork() const {
	return _pwd_nwk;
}

void				Server::setServinfo() {

    memset(&_hints, 0, sizeof _hints);
    _hints.ai_family = AF_UNSPEC;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_flags = AI_PASSIVE;
    if ((_status = getaddrinfo(_host.c_str(), _port.c_str(), &_hints, &_servinfo)) != 0) {
        throw eExc(gai_strerror(_status));
    }
}

void				Server::setSocket() {

    _socket = socket(_servinfo->ai_family,
                        _servinfo->ai_socktype,
                        _servinfo->ai_protocol);
    if (_socket == -1) {
        throw eExc(strerror(errno));
    }
}

ostream & operator<<(ostream & stream, Server &Server) {

    stream << "port: " << Server.getPort() << endl;
    stream << "pwd: " << Server.getPassword() << endl;
    stream << "host: " << Server.getHost() << endl;
    stream << "port network: " << Server.getPortNetwork() << endl;
    stream << "pwd network: " << Server.getPasswordNetwork() << endl;
    return stream;
}
