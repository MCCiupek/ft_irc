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
	for ( p = _servinfo; p != NULL; p = p->ai_next ) {
		if ( this->setSocket(p) )
			continue ;
		if ( this->bindPort(p) )
			continue ;
		break ;
	}
	freeaddrinfo(_servinfo);

	if ( !p ) {
		throw eExc("server: failed to bind");
	}
	this->listenHost();

	std::cout << BOLDGREEN  << "Server init success!!" << RESET << std::endl;
	std::cout << YELLOW << "Listening for clients ..." << RESET << std::endl;
}

void				Server::run() {

	int     fd_size = 5;
	char    buf[256];
	int     sender_fd = _sockfd;
	int     nbytes = 0;

	_poll = (struct pollfd *)malloc(sizeof *_poll * fd_size);
	_poll[0].fd = _sockfd;
	_poll[0].events = POLLIN;
	_fd_count = 1;

	for ( ;; ) {

		int poll_count = poll(_poll, _fd_count, -1);

		if (poll_count == -1) {
			throw eExc(strerror(errno));
		}

		for ( int i=0; i < _fd_count; i++ ) {

			if ( _poll[i].revents & POLLIN ) {

				if ( _poll[i].fd == _sockfd ) {
					this->acceptConn();
					add_to_pfds(&_poll, _newfd, &_fd_count, &fd_size);
				} else {
					// use htons/ntohs/etc to serialize/deserialize data bf send()/recv()
					nbytes = recv(_poll[i].fd, buf, sizeof buf, 0);
					sender_fd = _poll[i].fd;
					if ( nbytes <= 0 ) {
						if (nbytes == 0) {
							cout << "pollserver: socket " << sender_fd << " hung up" << endl;
						} else {
							throw eExc(strerror(errno));
						}
						close(_poll[i].fd);
						del_from_pfds(_poll, i, &_fd_count);
					}
				}
			} else {
				for ( int j = 0; j < _fd_count; j++ ) {
					int dest_fd = _poll[j].fd;
					if ( dest_fd != _sockfd && dest_fd != sender_fd ) {
						if ( send(dest_fd, buf, nbytes, 0) == -1 ) {
							throw eExc(strerror(errno));
						}
					}
				}
			}
		}
	}
}

void				Server::setServinfo() {

	cout << "Gathering server informations...";
	memset(&_hints, 0, sizeof _hints);
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	if ((_status = getaddrinfo(_host.c_str(), _port.c_str(), &_hints, &_servinfo)) != 0) {
		cout << RED << "KO" << RESET << endl;
		errno = _status;
		throw eExc("getaddrinfo: nodename nor servname provided, or not known");
	}
	cout << GREEN << "OK" << RESET << endl;
}

int				Server::setSocket( struct addrinfo * p ) {

	int yes = 1;

	cout << "Creating socket...";
	_sockfd = socket(p->ai_family,
						p->ai_socktype,
						p->ai_protocol);
	if (_sockfd == -1) {
		cout << RED << "KO" << RESET << endl;
		return 1;
	}
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		cout << RED << "KO" << RESET << endl;
		throw eExc(strerror(errno));
	}
	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1) {
		cout << RED << "KO" << RESET << endl;
		throw eExc(strerror(errno));
	}
	cout << GREEN << "OK" << RESET << endl;
	return 0;
}

int				Server::bindPort( struct addrinfo * p ) {

	cout << "Binding port " << _port << "...";
	if (bind(_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
		cout << RED << "KO" << RESET << endl;
		close(_sockfd);
		return 1;
	}
	cout << GREEN << "OK" << RESET << endl;
	return 0;
}

void				Server::listenHost() {

	cout << "listenning...";
	if (listen(_sockfd, BACKLOG) == -1) {
		cout << RED << "KO" << RESET << endl;
		throw eExc(strerror(errno));
	}
	cout << GREEN << "OK" << RESET << endl;
}

void				Server::receive() {
	
	char        buf[MAXBUFLEN];
	int         numbytes;
	socklen_t   addr_len;

	cout << "server: waiting to recv..." << endl;

	addr_len = sizeof _host_addr;
	if ((numbytes = recv(_sockfd, buf, MAXBUFLEN-1 , 0)) == -1) {
		throw eExc(strerror(errno));
	}
	buf[numbytes] = '\0';

	cout << "listener: packet contains \"" << buf << "\"" << endl;
}


void				Server::acceptConn() {

	socklen_t addr_size = sizeof _host_addr;
	_newfd = accept(_sockfd, (struct sockaddr *)&_host_addr, &addr_size);
	if ( _newfd == -1 ) {
		throw eExc(strerror(errno));
	}
	_s = inet_ntoa(get_in_addr((struct sockaddr *)&_host_addr));
	cout << "server: got connection from " << _s << endl;
}

ostream & operator<<(ostream & stream, Server &Server) {

	stream << "port: " << Server.getPort() << endl;
	stream << "pwd: " << Server.getPassword() << endl;
	stream << "host: " << Server.getHost() << endl;
	stream << "port network: " << Server.getPortNetwork() << endl;
	stream << "pwd network: " << Server.getPasswordNetwork() << endl;
	return stream;
}
