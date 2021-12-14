#include "headers.hpp"


/* String utils */

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

string ft_join(vector<string> str, string sep, int begin) {

	string  res;
	
	 for ( size_t i = begin; i < str.size(); i++ ) {
		res += str[i];
		if ( i != str.size() - 1 )
			res += sep;
	}
	return res;
}

bool ft_match( string const &str, string const &pattern ) {

	int n = str.size();
	int m = pattern.size();

	if (m == 0)
        return (n == 0);
 
    bool lookup[n + 1][m + 1];
    memset(lookup, false, sizeof(lookup));
 
    lookup[0][0] = true;
 
    for (int j = 1; j <= m; j++)
        if (pattern[j - 1] == '*')
            lookup[0][j] = lookup[0][j - 1];
 
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (pattern[j - 1] == '*')
                lookup[i][j] = lookup[i][j - 1] || lookup[i - 1][j];
            else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1])
                lookup[i][j] = lookup[i - 1][j - 1];
            else
                lookup[i][j] = false;
        }
    }
 
    return lookup[n][m];
}

void get_infos(const string &str, string &nickname, string &username, string &hostname) {
	
	//size_t			pos;
	//unsigned char	count = 0;
	(void)str;
	
	nickname = "*";
	username = "*";
	hostname = "*";
}

/* addr utils */

struct in_addr get_in_addr(struct sockaddr *sa) {
	
	return ((struct sockaddr_in*)sa)->sin_addr;
}


/* pollfd utils */

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


/* others */

ostream &operator<<(ostream & stream, const User &User) {

	/* This overload is only used for debug purpose and could be deleted */
	stream << "User: { ";
	stream << "fd: " << User.getFd() << ", ";
	stream << "username: " << User.getNick() << ", ";
	stream << "hostname: " << User.getUsername() << ", ";
	stream << "servername: " << User.getServername() << ", ";
	stream << "realname: " << User.getRealName() << ", ";
	stream << " }";
	return stream;
}