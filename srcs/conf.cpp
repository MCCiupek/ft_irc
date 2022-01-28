#include "headers.hpp"

static bool				valid_param( string name, string value )
{
	char	buf[BUFSIZE];

	if ((name == "PORT" && !is_digit(value)) || (name == "NAME" && !is_alpha(value))
		|| (name == "HOST" && !inet_pton(AF_INET, value.c_str(), buf)))
		return  false;
	if (name == "PORT" || name == "NAME" || name == "PASS" ||
		name == "MOTD" || name == "OPER" || name == "HOST")
		return true;
	
	return false;
}

map<string, string>		conf_file( char *path )
{
	ifstream	file(path);

	if (!file)
	{
		cerr << RED << "Configuration file can't be opened" << endl;
		exit(EXIT_FAILURE); 
	}

	map<string, string>		m;
	string					tmp;

	while (getline(file, tmp))
	{
		vector<string>	v = ft_split(tmp, "=");

		v[0] = trim(v[0]);
		if (v[1][0] != '\"')
			v[1] = trim(v[1]);
		else
			v[1] = trim(v[1], "\t\"");

		if (!valid_param(v[0], v[1]))
			throw eExc("Parameter in conf file is not valid");

		m[v[0]] = v[1];
	}

	file.close();

	return m;
}