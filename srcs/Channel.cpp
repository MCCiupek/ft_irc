#include "headers.hpp"

Channel::Channel( void ) {
	vector<string> banned_nicks;
	vector<string> banned_usernames;
	vector<string> banned_hostnames;

	_banned["nick"] = banned_nicks;
	_banned["user"] = banned_usernames;
	_banned["host"] = banned_hostnames;
}

Channel::Channel(string name) :
		_name(TRUNC(name, MAX_CHAN_NAME_LEN)),
		_key(""),
		_has_key(false),
		_topic(""),
		_has_topic(false),
		_mode("")
{
	vector<string> banned_nicks;
	vector<string> banned_usernames;
	vector<string> banned_hostnames;

	_banned["nick"] = banned_nicks;
	_banned["user"] = banned_usernames;
	_banned["host"] = banned_hostnames;
}

Channel::Channel(string name, string key, string topic, User * usr, string mode) : 
		_name(TRUNC(name, MAX_CHAN_NAME_LEN)),
		_key(key),
		_has_key(false),
		_topic(topic),
		_has_topic(false),
		_mode(mode)
{
	vector<string> banned_nicks;
	vector<string> banned_usernames;
	vector<string> banned_hostnames;

	_banned["nick"] = banned_nicks;
	_banned["user"] = banned_usernames;
	_banned["host"] = banned_hostnames;

	if ( key != "" )
		_has_key = true;
	
	if ( topic != "" )
		_has_topic = true;

	_channel_oper = usr;

	_members.push_back(usr);

	string usr_mode = usr->getMode();
	if (usr_mode.find('o') == string::npos)
		usr->setMode(usr_mode + "o");
}

Channel::~Channel() {
	
}

Channel & Channel::operator=(Channel const & src) {

	if (this != &src) {
		this->_name = src.getName();
		this->_has_key = src.getHasKey();
		this->_key = src.getKey();
		this->_has_topic = src.getHasTopic();
		this->_topic = src.getTopic();
		this->_mode = src.getMode();
	}
	return *this;
}

string const		&Channel::getName() const {
	return _name;
}

string const		&Channel::getKey() const {
	return _key;
}

bool const			&Channel::getHasKey() const {
	return _has_key;
}

size_t				Channel::getNbMembers() const {
	return _members.size();
}

string const		&Channel::getTopic() const {
	return _topic;
}

bool const			&Channel::getHasTopic() const {
	return _has_topic;
}

vector<User*> const	&Channel::getMembers() const {
	return _members;
}

User *				Channel::getOperator( void ) {
	return _channel_oper;
}

string const		&Channel::getMode( void ) const {
	return _mode;
}

void    			Channel::setName(string const & name) {
	_name = name;
}

void    			Channel::setKey(string const & key) {
	_key = key;
	_has_key = true;
}

void    			Channel::setTopic(string const & topic) {
	_topic = topic;
	_has_topic = true;
}

void    			Channel::unsetTopic() {
	_topic = "";
}

void				Channel::setMode( string mode ) {
	_mode = mode;
}

void				Channel::addMember( User * usr ) {
	_members.push_back(usr);
}

void				Channel::deleteMember( User * usr ) {
	
	for ( size_t i = 0; i < this->getNbMembers(); i++ ) {
		if ( usr->getNick() == _members[i]->getNick() )
			_members.erase(_members.begin() + i);
	}
}

void				Channel::ban( string to_ban, string key ) {

	if ( _banned.find(key) != _banned.end() )
		_banned[key].push_back(to_ban);
}

bool				Channel::isBanned( User const & usr ) {

	if ( find( _banned["nick"].begin(), _banned["nick"].end(), usr.getNick()) != _banned["nick"].end() )
		return true;
	if ( find( _banned["user"].begin(), _banned["user"].end(), usr.getUsername()) != _banned["user"].end() )
		return true;
	if ( find( _banned["host"].begin(), _banned["host"].end(), usr.getHostname()) != _banned["host"].end() )
		return true;
	return false;
}

void				Channel::invite( User * usr ) {
	_invited_usrs.push_back(usr);
}

bool				Channel::isInvited( User const & usr ) {

	if ( find( _invited_usrs.begin(), _invited_usrs.end(), &usr) != _invited_usrs.end() )
		return true;
	return false;
}

bool					Channel::isInviteOnly( void ) {

	return this->getMode().find("i") != string::npos;
}

string				Channel::getMembersList( void ) {

	string reply;

	for ( size_t i = 0; i < _members.size(); i++ ) {
		reply += _members[i]->getNick();
		if ( i < _members.size() - 1 )
			reply += " ";
	}
	return reply;
}

bool				Channel::isOnChann( User const & usr ) {
	for ( size_t i = 0; i < _members.size(); i++ ) {
		if ( usr.getNick() == _members[i]->getNick())
			return true;
	}
	return false;
}

string		Channel::MembersToString( User const & u, Server const & srv ) {

	ostringstream s;

	// irssi syntax [ :<server> 353 <nickname> = <channel> :@<channels_users> ]
	s	<< ":" << srv.getName() << " 353 " << u.getNick()
		<< " = " << _name << " :@";

	for (vector<User *>::iterator it = _members.begin(); it != _members.end(); it++)
		s << (*it)->getNick() << " ";

	s << "\r\n";

	// irssi syntax [ :<server> 324 <nickname> <channel> :<message> ]
	s	<< ":" << srv.getName() << " 366 " << u.getNick()
		<< " " << _name << " :End of /NAMES list." << "\r\n";

	return (s.str());
}

ostream & operator<<(ostream & stream, Channel &Channel) {

	stream << "Channel: " << Channel.getName();
	stream << " created by " << Channel.getOperator()->getNick();
	if (Channel.getHasKey())
		stream << " is private (key: " << Channel.getKey() << ")";
	else
		stream << " is public";
	return stream;

}
