#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "headers.hpp"

# define ERR_NOSUCHSERVER		402
# define ERR_NOORIGIN			409
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462

class eExc : public exception {
	
	public:

		eExc(const char * str) : _strerror(str) {};
		const char * what() const throw() { return _strerror; };

	private:

		const char * _strerror;
};

extern map<int, string>		err;	// errors list

int		display_usage( void );
void    define_errors( void );
void    send_error( User u, int errn, string cmd );

#endif
