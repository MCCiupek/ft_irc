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

Server::Server(string port, string pwd, string host="localhost",
	string port_nwk="127.0.0.1", string pwd_nwk="") : 
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

map<int, User> &Server::getUsers() {
	return _users;
}

ostream & operator<<(ostream & stream, Server &Server) {

	stream << "port: " << Server.getPort() << endl;
	stream << "pwd: " << Server.getPassword() << endl;
	stream << "host: " << Server.getHost() << endl;
	stream << "port network: " << Server.getPortNetwork() << endl;
	stream << "pwd network: " << Server.getPasswordNetwork() << endl;
	return stream;
}

void				Server::listenHost() {

	cout << "listenning...";
	if (listen(_sockfd, BACKLOG) == -1) {
		cout << RED << "KO" << RESET << endl;
		throw eExc(strerror(errno));
	}
	cout << GREEN << "OK" << RESET << endl;
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

void				Server::setServinfo() {

	cout << "Gathering server informations...";
	memset(&_hints, 0, sizeof _hints);
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	if ((_status = getaddrinfo(_host.c_str(), _port.c_str(), &_hints, &_servinfo)) != 0) {
		cout << RED << "KO" << RESET << endl;
		errno = _status;
		throw eExc("getaddrinfo: nodename nor servname provided, or not known");
	}
	cout << GREEN << "OK" << RESET << endl;
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

int				Server::sendData( int fd ) {
	
	int		dest_fd;
	char    buf[BUFSIZE];

	for ( int j = 0; j < _fd_count; j++ ) {
		dest_fd = _poll[j].fd;
		if ( dest_fd != _sockfd && dest_fd != fd )
			if ( send(dest_fd, buf, BUFSIZE - 1, 0) == -1 )
				throw eExc(strerror(errno));
	}
	return 0;
}

int				Server::receiveData( int i ) {
	
	char    		buf[BUFSIZE];
	int 			nbytes;
	ostringstream	s;
	string			nick = _users[_poll[i].fd].getNick();

	if (nick == "") {
		s << "Client #" << _poll[i].fd;
		nick = s.str();
	}
	memset(buf, 0, BUFSIZE);
	nbytes = recv(_poll[i].fd, buf, BUFSIZE - 1, 0);
	if (nbytes <= 0) {
		if (nbytes == 0)
			cout << BOLDWHITE << "❌ " << nick << " gone away" << RESET << endl;
			//cout << BOLDWHITE << "❌ Client #" << _poll[i].fd << " gone away" << RESET << endl;
		close(_poll[i].fd);
		del_from_pfds(_poll, i, &_fd_count);
		if (nbytes < 0)
			throw eExc(strerror(errno));
		return 1;
	}

	if (parsing(ft_split(buf, " "), _users[_poll[i].fd], *this))
		return 1;
	
	//cout << "Client #" << _poll[i].fd << ": " << buf;
	cout << nick << ": " << buf;
	return 0;
}

void				Server::acceptConn() {

	struct sockaddr_in	host_addr;

	socklen_t addr_size = sizeof host_addr;
	_newfd = accept(_sockfd, (struct sockaddr *)&host_addr, &addr_size);
	if ( _newfd == -1 ) {
		throw eExc(strerror(errno));
	}

	// inet_ntoa()
	// function converts the Internet host address in, given in network
	// byte order, to a string in IPv4 dotted-decimal notation.
	cout << BOLDWHITE << "✅ New client #" << _newfd
		 << " from " << inet_ntoa(host_addr.sin_addr)
		 << ":" << ntohs(host_addr.sin_port) << RESET << endl;
}

void				Server::run() {

	int     fd_size = MAXCLI;

	_poll = (struct pollfd *)malloc(sizeof *_poll * fd_size);
	_poll[0].fd = _sockfd;
	_poll[0].events = POLLIN;
	_fd_count = 1;

	while (1) {

		int poll_count = poll(_poll, _fd_count, -1);

		if (poll_count == -1)
			throw eExc(strerror(errno));

		for ( int i = 0; i < _fd_count; i++ ) {
			// If something happened on fd i
			if ( _poll[i].revents & POLLIN ) {
				// New connection / New user
				if ( _poll[i].fd == _sockfd )
				{
					this->acceptConn();
					// Add new fd that made the connection
					add_to_pfds(&_poll, _newfd, &_fd_count, &fd_size);
					// Register new user
					// cout << "new user = " << _newfd << endl;
					//_users[i + 1] = User(_newfd);
					_users[_newfd] = User(_newfd);
					break ;
				}
				else
				{
					// cout << "fd = " << i << endl;
					if (!this->receiveData(i))
						this->sendData( _poll[i].fd );
				}
			}
		}
	}
}

int				Server::is_registered( User usr )
{
	//cout << _users << endl;
	for (map<int, User>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
		if ((it->second).getFd() == usr.getFd())
			return it->first;
	}
	//if (_users.find(usr.getFd()) != _users.end())
	//	return true;
	
	return -1;
}

Channel *				Server::getChannelByName( string channel ) {

	for (vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if ( (*it)->getName() == channel )
			return *it;
	}
	return NULL;
}

Channel *				Server::getChannelByKey( string key ) {

	for (vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if ( (*it)->getHasKey() && (*it)->getKey() == key )
			return *it;
	}
	return NULL;
}

User *				Server::getUserByNick( string nick ) {

	for (map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it) {
		User * usr = &it->second;
		if ( usr->getNick() == nick )
			return usr;
	}
	return NULL;
}