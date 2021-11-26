#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "headers.hpp"

class eExc : public exception {
	
	public:

		eExc(string str) : _strerror(str.c_str()) {};
		const char * what() const throw() { return _strerror; };

	private:

		const char * _strerror;
};

int display_usage( void );

#endif
