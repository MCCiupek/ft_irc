#include "headers.hpp"

Server::Server(int port, string pwd) : 
        _port(port), 
        _pwd(pwd)
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
