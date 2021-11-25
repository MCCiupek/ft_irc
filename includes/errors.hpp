#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "headers.hpp"

class eUsage : public std::exception { 
	public:
		const char * what() const throw() { return "Usage: ./ircserv [host:port_network:password_network] <port> <password>"; } };

class ePort : public std::exception { 
	public:
		const char * what() const throw() { return "Port must be a positive integer."; } };


int display_usage( void );

#endif
