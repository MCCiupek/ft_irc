#ifndef HEADERS_HPP
# define HEADERS_HPP

# define BACKLOG 5
# define MAXCLI 5
# define BUFSIZE 128

# include <iostream>
# include <vector>
# include <map>
# include <string>
# include <exception>
# include <sstream>
# include <iterator>
# include <cerrno>
# include <cstring>

# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <signal.h>
# include <system_error>
# include <fcntl.h>
# include <poll.h>

using namespace std;

# include "User.hpp"
# include "utils.hpp"
# include "colors.hpp"
# include "errors.hpp"
# include "parsing.hpp"
# include "Server.hpp"

void		nick( vector<string> args, User &usr, Server &srv );
void		user( vector<string> args, User &usr, Server &srv );

#endif
