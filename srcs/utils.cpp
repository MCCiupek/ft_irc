#include "headers.hpp"

vector<string> ft_split(string str, string sep) {

    vector<string>  res;
    string          tok;

    size_t     start = 0;
    size_t     end = str.find(sep);
    while (end != string::npos) {
        res.push_back(str.substr(start, end - start));
        start = end + sep.length();
        end = str.find(sep, start);
    }
    res.push_back(str.substr(start, end - start));
    return res;
}

struct in_addr get_in_addr(struct sockaddr *sa) {
    
    return ((struct sockaddr_in*)sa)->sin_addr;
}

void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size) {
    
    if ( *fd_count == *fd_size ) {
        *fd_size *= 2;
        *pfds = (struct pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN;

    (*fd_count)++;
}

void del_from_pfds(struct pollfd pfds[], int i, int *fd_count) {
    pfds[i] = pfds[*fd_count-1];
    (*fd_count)--;
}

ostream &operator<<(ostream & stream, const User &User) {

	stream << "User: { ";
	stream << "fd: " << User.getFd() << ", ";
	stream << "username: " << User.getNick() << ", ";
	stream << "hostname: " << User.getUsername() << ", ";
	stream << "servername: " << User.getServername() << ", ";
	stream << "realname: " << User.getRealName() << ", ";
	stream << " }";
	return stream;
}