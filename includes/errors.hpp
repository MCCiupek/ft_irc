#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "headers.hpp"

class eExc : public exception {
	
	public:

		eExc(const char * str) : _strerror(str) {};
		const char * what() const throw() { return _strerror; };

	private:

		const char * _strerror;
};

int display_usage( void );

#endif
