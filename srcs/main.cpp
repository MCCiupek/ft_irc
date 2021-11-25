#include "headers.hpp"

int main( int argc, char *argv[] ) {
    if ( argc != 4 ) {
        return display_usage();
    }
    int     port = atoi(argv[2]);
    string  pwd = argv[3];
    vector<string> v = ft_split(argv[1], ":");
    for (size_t i=0; i < v.size(); i++) {
        cout << v[i] << endl; 
    }
    cout << "port: " << port << endl;
    cout << "pwd: " << pwd << endl;
    return 0;
}