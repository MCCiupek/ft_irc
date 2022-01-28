#ifndef HEADERS_HPP
# define HEADERS_HPP

# define SERVER_NAME        "mfirc" 
# define DEFAULT_HOST       "127.0.0.1"
# define AVAILABLE_USER_MODES "iswo"
# define AVAILABLE_CHANNEL_MODES "opsitnmlbvk"
# define BACKLOG			5
# define MAXCLI				5
# define BUFSIZE			128
# define SERVER_VERSION		"0.7.13"
# define MAX_CHAN_PER_USR	10
# define MAX_USR_PER_CHAN	10
# define MAX_USR_NICK_LEN	20
# define MAX_CHAN_NAME_LEN	200

# ifdef __APPLE__
#  define INTMAX_T intmax_t
# elif __linux__
#  define INTMAX_T __intmax_t
# endif

# define TRUNC(str, len) (str.substr(0, min(name.length(), (unsigned long)len)))

# include <iostream>
# include <vector>
# include <map>
# include <string>
# include <exception>
# include <sstream>
# include <iterator>
# include <cerrno>
# include <cstring>
# include <cstdlib>
# include <ctime>
# include <algorithm>
# include <fstream>
# include <iomanip>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <signal.h>
# include <fcntl.h>
# include <poll.h>

using namespace std;

# include "colors.hpp"
# include "User.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "utils.hpp"
# include "errors.hpp"
# include "parsing.hpp"
# include "cmd.hpp"

#endif
