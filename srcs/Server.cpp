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

    struct addrinfo * p;

    this->setServinfo();
    for( p = _servinfo; p != NULL; p = p->ai_next ) {
        if (this->setSocket(p))
            continue ;
        if (this->bindPort(p))
            continue ;
        break ;
    }
    freeaddrinfo(_servinfo);

    if (!p)
        throw eExc("server: failed to bind");
    this->listenHost();
    this->setSA();

    cout << "server: waiting for connections..." << endl;

    while (42) {

        this->acceptConn();
    }
}

void				Server::setServinfo() {

    cout << "Gathering server informations...";
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

int				Server::setSocket( struct addrinfo * p ) {

    int yes=1;

    cout << "Creating socket...";
    _sockfd = socket(p->ai_family,
                        p->ai_socktype,
                        p->ai_protocol);
    if (_sockfd == -1) {
        return 1;
    }
    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            throw eExc(strerror(errno));
    }
    cout << "OK" << endl;
    return 0;
}

int				Server::bindPort( struct addrinfo * p ) {

    cout << "Binding port " << _port << "...";
    if (bind(_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(_sockfd);
        return 1;
    }
    cout << "OK" << endl;
    return 0;
}

void				Server::listenHost() {

    cout << "listenning...";
    if (listen(_sockfd, BACKLOG) == -1) {
        throw eExc(strerror(errno));
    }
    cout << "OK " << endl;
}


void				Server::setSA() {
    
    _sa.sa_handler = sigchld_handler;
    sigemptyset(&_sa.sa_mask);
    _sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &_sa, NULL) == -1) {
        throw eExc(strerror(errno));
    }
}

void				Server::acceptConn() {

    socklen_t addr_size = sizeof _host_addr;
    _newfd = accept(_sockfd, (struct sockaddr *)&_host_addr, &addr_size);
    if (_newfd == -1) {
        throw eExc(strerror(errno));
    }
    _s = inet_ntoa(get_in_addr((struct sockaddr *)&_host_addr));
    cout << "server: got connection from " << _s << endl;
    close(_newfd);
}

ostream & operator<<(ostream & stream, Server &Server) {

    stream << "port: " << Server.getPort() << endl;
    stream << "pwd: " << Server.getPassword() << endl;
    stream << "host: " << Server.getHost() << endl;
    stream << "port network: " << Server.getPortNetwork() << endl;
    stream << "pwd network: " << Server.getPasswordNetwork() << endl;
    return stream;
}
