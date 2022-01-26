#include "headers.hpp"

Server::Server( void ) {}

Server::Server(string port, string pwd) :
		_name(SERVER_NAME),
		_port(port), 
		_pwd(pwd),
		_host(DEFAULT_HOST),
		_servinfo(NULL),
		_users(),
		_irc_operators(),
		_motd("")
{
	time_t now = time(0);
	_creation_date = ctime(&now);
	pop_back(_creation_date);
	
}

Server::Server(string port, string pwd, string host=DEFAULT_HOST, string motd="",
			string operators="") : 
		_name(SERVER_NAME),
		_port(port), 
		_pwd(pwd),
		_host(host),
		_servinfo(NULL),
		_users(),
		_motd(motd)
{
	time_t now = time(0);
	_creation_date = ctime(&now);
	vector<string>	cred = ft_split(operators, "|");

	for (vector<string>::iterator it = cred.begin(); it != cred.end(); ++it)
	{
		vector<string>	tmp = ft_split(*it, ":");

		_irc_operators[tmp[0]] = tmp[1];
	}
}

Server::~Server() {
	
}

Server 						&Server::operator=(Server const & src) {

	if (this != &src) {
		this->_name = src.getName();
		this->_port = src.getPort();
		this->_pwd = src.getPassword();
		this->_host = src.getHost();
		this->_motd = src.getMotd();
		this->_users = src.getUsers();
	}
	return *this;
}

string const 				&Server::getName() const {
	return _name;
}

string const 				&Server::getPort() const {
	return _port;
}

string const 				&Server::getPassword() const {
	return _pwd;
}

string const 				&Server::getHost() const {
	return _host;
}

map<int, User> const 		&Server::getUsers() const {
	return _users;
}

vector<Channel*> const 		&Server::getChannels( void ) const {
	return _channels;
}

vector<string> const		Server::getChannelsNames( void ) const {

	vector<string> names;

	for ( size_t i = 0; i < _channels.size(); i++ )
		names.push_back(_channels[i]->getName());
	return names;
}

string const 				&Server::getMotd() const {
	return _motd;
}

string const 				&Server::getCreationDate() const {
	return _creation_date;
}

map<string, string>	const	&Server::getIRCOperators() const {
	return _irc_operators;
}

ostream & operator<<(ostream & stream, Server &Server) {

	stream << "name: " << Server.getName() << endl;
	stream << "port: " << Server.getPort() << endl;
	stream << "pwd: " << Server.getPassword() << endl;
	stream << "host: " << Server.getHost() << endl;
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

vector<string>  get_next_command( char *buf )
{
	static string   buffer = "";
	string			s(buffer + buf);

	// Multiple commands on one line
	if (count(s.begin(), s.end(), '\n') > 0)
	{
		buffer = "";
		vector<string> tmp = ft_split(s, "\n");
		
		// Delete \r in case of connection from irssi
		for (vector<string>::iterator it = tmp.begin(); it != tmp.end(); ++it)
			if ((*it)[0] == '\r')
				(*it).erase(0, 1);

		return tmp;
	}
	else
		buffer += buf;

	return vector<string>();
}

int				Server::receiveData( int i ) {
	
	char    		buf[BUFSIZE];
	int 			nbytes;
	ostringstream	s;
	string			nick = _users[_poll[i].fd].getNick();

	cout << getIRCOperators().size() << endl;

	if (nick == "") {
		s << "Client #" << _poll[i].fd;
		nick = s.str();
	}
	memset(buf, 0, BUFSIZE);
	nbytes = recv(_poll[i].fd, buf, BUFSIZE - 1, 0);
	if (nbytes <= 0) {
		if (!nbytes)
			cout << BOLDWHITE << "❌ Client #" << _poll[i].fd << " gone away" << RESET << endl;
		del_from_pfds(i);
		if (nbytes < 0)
			throw eExc(strerror(errno));
		return 1;
	}

	cout << "buf = " << buf << endl;

	vector<string>  v = get_next_command(buf);
	if (!v.empty())
		v.pop_back(); // Delete last empty line

	for (vector<string>::iterator it = v.begin(); it != v.end(); it++)
		cout << "vec " << *it << endl;

	if (v.size() > 0)
		for (vector<string>::iterator it = v.begin(); it != v.end(); it++)
			parsing(ft_split(*it, " "), _users[_poll[i].fd], *this);

	// cout << nick << ": " << buf;
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

/* pollfd utils */

void				Server::add_to_pfds(int newfd)
{
	
	// if ( _fd_count == *fd_size ) {
	// 	*fd_size *= 2;
	// 	_poll = (struct pollfd *)realloc(_poll, sizeof(*_poll) * (*fd_size));
	// }

	if (_fd_count == 5)
		cout << RED << "Max number of clients reached" << RESET << endl;
	else
	{
		(_poll)[_fd_count].fd = newfd;
		(_poll)[_fd_count].events = POLLIN;

		(_fd_count)++;
	}
}

void				Server::del_from_pfds(int fd)
{
	close(fd);
	_poll[fd] = _poll[_fd_count - 1];
	_fd_count--;
}

void				Server::run() {

	// int     fd_size = MAXCLI;

	// _poll = (struct pollfd *)malloc(sizeof *_poll * fd_size);
	_poll[0].fd = _sockfd;
	_poll[0].events = POLLIN;
	_fd_count = 1;

	while (1) {

		int poll_count = poll(_poll, _fd_count, -1);

		if (poll_count == -1)
			throw eExc(strerror(errno));

		//cout << "fd_count = " << _fd_count << endl;

		for ( int i = 0; i < _fd_count; i++ ) {
			// If something happened on fd i
			if ( _poll[i].revents & POLLIN ) {
				// New connection / New user
				if ( _poll[i].fd == _sockfd )
				{
					this->acceptConn();
					// Add new fd that made the connection (Up to 5)
					add_to_pfds(_newfd);
					// Create new user
					_users[_newfd] = User(_newfd);
					break ;
				}
				else
					this->receiveData(i);
			}
		}
	}
}

//	If fd was registered
bool					Server::is_registered( User usr )
{
	for (map<int, User>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
		if ((it->second).getFd() == usr.getFd())
			return true;
	}
	
	return false;
}

bool					Server::username_isIRCOper( string usr_name )
{
	for (map<string, string>::iterator it = _irc_operators.begin(); it != _irc_operators.end(); ++it) {
		if (it->first == usr_name)
			return true;
	}

	return false;	
}

bool					Server::isIRCOperator( string usr_name, string pswd )
{
	for (map<string, string>::iterator it = _irc_operators.begin(); it != _irc_operators.end(); ++it) {
		if (it->first == usr_name && it->second == pswd)
			return true;
	}

	return false;
}

Channel *				Server::getChannelByName( string channel ) {

	// cout << "getChannelByName" << endl;

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

void				Server::addChannel( Channel * channel ) {
	
	_channels.push_back(channel);
}

void				Server::deleteChannel( Channel * channel ) {

	for ( vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++ ) {
		if ( (*it)->getName() == channel->getName() ) {
			_channels.erase(it);
			return ;
		}
	}
}