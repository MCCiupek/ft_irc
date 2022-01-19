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
	send_reply(usr, 001, RPL_WELCOME(usr.getNick(), usr.getUsername(), usr.getHostname()));
	send_reply(usr, 002, RPL_YOURHOST(srv.getName(), SERVER_VERSION));
	send_reply(usr, 003, RPL_CREATED(srv.getCreationDate()));
	send_reply(usr, 004, RPL_MYINFO(srv.getName(), SERVER_VERSION, AVAILABLE_USER_MODES, AVAILABLE_CHANNEL_MODES));

	string motd = srv.getMotd();

	if (!motd.empty())
	{
		send_reply(usr, 375, RPL_MOTDSTART(srv.getName()));

		vector<string>	tmp = ft_split(motd, "\\n");

		for (vector<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
			send_reply(usr, 372, RPL_MOTD(*it));
		
		send_reply(usr, 376, RPL_ENDOFMOTD());
	}
	else
		send_error(usr, ERR_NOMOTD, "");
}