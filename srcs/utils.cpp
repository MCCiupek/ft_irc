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
