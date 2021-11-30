#include "headers.hpp"

int display_usage( void ) {
    
    cout << "Usage: ./ircserv [host:port_network:password_network] <port> <password>" << endl;
    return 1;
}

void sigchld_handler(int s) {

    (void)s;
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}
