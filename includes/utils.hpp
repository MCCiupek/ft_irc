#ifndef UTILS_HPP
# define UTILS_HPP

# include "headers.hpp"


string			ft_join(vector<string> str, string sep, int begin=0);
bool			ft_match( string const &str, string const &pattern );

void get_infos(const string &str, string &nickname, string &username, string &hostname);

# include "Server.hpp"

string				to_upper( string s );
bool				is_digit( string s );
bool				is_alpha( string s );
bool				is_alnum( string s );
bool				is_upper( string s );
string				trim(const string& str, const string& whitespace = " \t");
vector<string>  	ft_split(string str, string sep);
struct in_addr  	get_in_addr(struct sockaddr *sa);
void 				add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size);
void 				del_from_pfds(struct pollfd pfds[], int i, int *fd_count);
void				messageoftheday( Server &srv, User usr );

ostream				&operator<<(ostream & stream, User const &User);

template<typename K, typename V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
	for(typename map<K, V>::const_iterator it = m.begin(); it != m.end(); ++it)
		os << it->first << ": " << it->second << endl;
	return os;
}

#endif
