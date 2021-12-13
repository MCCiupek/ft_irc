#include "headers.hpp"

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

string			server_msg( Server &srv )
{
	return ("[server:" + srv.getPort() + "]: ");
}

string			get_time()
{
	ostringstream str;

    time_t now = time(0);

	return (ctime(&now));
}

void    		messageoftheday( Server &srv, User usr )
{
	ostringstream str;

    str << server_msg(srv) << srv.getName() << " 001 " << usr.getNick()
		<< " :Welcome to the Internet Relay Network " << usr.getNick() << endl
		<< ":" << srv.getName() << " 002 " << usr.getNick()
		<< " :Your host is " << srv.getName() << ", running " << SERVER_VERSION << endl
		<< ":" << srv.getName() << " 003 " << usr.getNick()
		<< " :This server was created " << get_time()
		<< ":" << srv.getName() << " 004 " << usr.getNick()
		<< " :" << srv.getName() << " " << SERVER_VERSION << endl;

	if ( send(usr.getFd(), &str.str()[0], str.str().size(), 0) == -1 )
		throw eExc(strerror(errno));

	// Reset stringstream
	str.str("");
	str.clear();

	string motd = srv.getMotd();

	if (!motd.empty())
	{
		str << ":" << srv.getName() << " 375 " << usr.getNick()
		<< " :- " << srv.getName() << " Message of the day -" << endl;

		vector<string>	tmp = ft_split(motd, "\\n");

		for (vector<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		{
			str << ":" << srv.getName() << " 372 " << usr.getNick()
				<< " :" << *it << endl;
		}
		str << ":" << srv.getName() << " 372 " << usr.getNick()
			<< " :End of MOTD command" << endl;

		if ( send(usr.getFd(), &str.str()[0], str.str().size(), 0) == -1 )
			throw eExc(strerror(errno));
	}
	else
		send_error(usr, ERR_NOMOTD, "");
}