#include "headers.hpp"


/* String utils */

string				to_upper( string s )
{
	for (string::iterator it = s.begin(); it != s.end(); it++)
		*it -= 32;
	
	return (s);
}

bool		is_digit( string s )
{
	for (string::iterator it = s.begin(); it != s.end(); it++)
		if (!isdigit(*it))
			return false;
	
	return true;
}

bool		is_alpha( string s )
{
	for (string::iterator it = s.begin(); it != s.end(); it++)
		if (!isalpha(*it))
			return false;
	
	return true;
}

bool		is_alnum( string s )
{
	for (string::iterator it = s.begin(); it != s.end(); it++)
		if (!isalnum(*it))
			return false;
	
	return true;
}

bool		is_upper( string s )
{
	for (string::iterator it = s.begin(); it != s.end(); it++)
		if (!isupper(*it))
			return false;
	
	return true;
}

string				trim(const string& str, const string& whitespace)
{
    const size_t strBegin = str.find_first_not_of(whitespace);

    if (strBegin == string::npos)
        return ("");

    const size_t strEnd = str.find_last_not_of(whitespace);
    const size_t strRange = strEnd - strBegin + 1;

    return (str.substr(strBegin, strRange));
}


vector<string>		ft_split(string str, string sep) {

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

string			get_time()
{
	ostringstream str;

    time_t now = time(0);

	return (ctime(&now));
}

void    		messageoftheday( Server &srv, User usr )
{
<<<<<<< HEAD
=======
	// ostringstream str;

>>>>>>> d60ad405d52e694cc5204dfa7b05d9445be0fbd6
	send_reply(usr, 001, RPL_WELCOME(usr.getNick(), usr.getUsername(), usr.getHostname()));
	send_reply(usr, 002, RPL_YOURHOST(srv.getName(), SERVER_VERSION));
	send_reply(usr, 003, RPL_CREATED(srv.getCreationDate()));
	send_reply(usr, 004, RPL_MYINFO(srv.getName(), SERVER_VERSION, AVAILABLE_USER_MODES, AVAILABLE_CHANNEL_MODES));

<<<<<<< HEAD
	string motd = srv.getMotd();
=======
	// Reset stringstream
	// str.str("");
	// str.clear();

	string motd = MOTD;//srv.getMotd();
>>>>>>> d60ad405d52e694cc5204dfa7b05d9445be0fbd6

	if (!motd.empty())
	{
		send_reply(usr, 375, RPL_MOTDSTART(srv.getName()));

		vector<string>	tmp = ft_split(motd, "\\n");
		ostringstream	str;

		for (vector<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		{
			if (it != tmp.begin())	// send_reply add the first
				str << ":" << srv.getName() << " 372 " << usr.getNick() << " ";
			str	<< ":" << *it << "\r\n";
		}
		
		send_reply(usr, 372, RPL_MOTD(str.str()));
		send_reply(usr, 376, RPL_ENDOFMOTD());
	}
	else
		send_error(usr, ERR_NOMOTD, "");
}