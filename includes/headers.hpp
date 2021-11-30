#ifndef HEADERS_HPP
# define HEADERS_HPP

# include <iostream>
# include <vector>
# include <map>
# include <string>
# include <exception>

# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <system_error>
# include <fcntl.h>
# include <poll.h>

using namespace std;

# include "utils.hpp"
# include "colors.hpp"
# include "errors.hpp"
# include "parsing.hpp"

# include "Server.hpp"

#endif
