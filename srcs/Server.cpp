#include "headers.hpp"

Server::Server( void ) {

}

Server::Server(string port, string pwd) : 
        _port(port), 
        _pwd(pwd),
        _host("localhost"),
        _port_nwk("127.0.0.1"),
        _pwd_nwk(""),
        _servinfo(NULL)
{

}

Server::Server(string port, string pwd, string host="localhost", string port_nwk="0", string pwd_nwk="") : 
        _port(port), 
        _pwd(pwd),
        _host(host),
        _port_nwk(port_nwk),
        _pwd_nwk(pwd_nwk),
        _servinfo(NULL)
{

}

Server::~Server() {
    
    if ( _servinfo ) {
        freeaddrinfo(_servinfo);
    }
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

void				Server::initConn() {

    this->setServinfo();
    this->setSocket();
    this->bindPort();
    this->listenHost();
    this->acceptConn();
}

void				Server::setServinfo() {

    cout << "Setting-up server informations...";
    memset(&_hints, 0, sizeof _hints);
    _hints.ai_family = AF_UNSPEC;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_flags = AI_PASSIVE;
    if ((_status = getaddrinfo(_host.c_str(), _port.c_str(), &_hints, &_servinfo)) != 0) {
        errno = _status;
        cout << gai_strerror(_status) << endl;
        throw eExc(gai_strerror(_status));
    }
    cout << "OK" << endl;
}

void				Server::setSocket() {

    cout << "Setting-up socket...";
    _sockfd = socket(_servinfo->ai_family,
                        _servinfo->ai_socktype,
                        _servinfo->ai_protocol);
    if (_sockfd == -1) {
        throw eExc(strerror(errno));
    }
    cout << "OK" << endl;
}

void				Server::bindPort() {

    cout << "Binding port " << _port_nwk << "...";
    if (bind(_sockfd, _servinfo->ai_addr, _servinfo->ai_addrlen) == -1) {
        throw eExc(strerror(errno));
    }
    cout << "OK" << endl;
}

void				Server::listenHost() {

    cout << "listenning...";
    if (listen(_sockfd, BACKLOG) == -1) {
        throw eExc(strerror(errno));
    }
    cout << "OK" << endl;
}

void				Server::acceptConn() {

    cout << "accepting connexion...";
    socklen_t addr_size = sizeof _host_addr;
    _newfd = accept(_sockfd, (struct sockaddr *)&_host_addr, &addr_size);
    if (_newfd == -1) {
        throw eExc(strerror(errno));
    }
    cout << "OK" << endl;
}

ostream & operator<<(ostream & stream, Server &Server) {

    stream << "port: " << Server.getPort() << endl;
    stream << "pwd: " << Server.getPassword() << endl;
    stream << "host: " << Server.getHost() << endl;
    stream << "port network: " << Server.getPortNetwork() << endl;
    stream << "pwd network: " << Server.getPasswordNetwork() << endl;
    return stream;
}
