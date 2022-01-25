#include "headers.hpp"

Channel::Channel( void ) {
	vector<string> banned_nicks;
	vector<string> banned_usernames;
	vector<string> banned_hostnames;

	_banned["nick"] = banned_nicks;
	_banned["user"] = banned_usernames;
	_banned["host"] = banned_hostnames;

	time_t now = time(0);
	_creation_date = (intmax)now;

	_topic_when = (intmax)now;
	_topic_who = NULL;
}

Channel::Channel(string name) :
		_name(TRUNC(name, MAX_CHAN_NAME_LEN)),
		_key(""),
		_has_key(false),
		_topic(""),
		_has_topic(false),
		_mode(""),
		_limit(MAX_USR_PER_CHAN)
{
	vector<string> banned_nicks;
	vector<string> banned_usernames;
	vector<string> banned_hostnames;

	_banned["nick"] = banned_nicks;
	_banned["user"] = banned_usernames;
	_banned["host"] = banned_hostnames;

	time_t now = time(0);
	_creation_date = (intmax)now;

	_topic_when = (intmax)now;
	_topic_who = NULL;
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

	time_t now = time(0);
	_creation_date = (intmax)now;

	if ( key != "" ) {
		_has_key = true;
		_mode += "k";
	}

	_topic_when = (intmax)now;
	_topic_who = usr;
	
	if ( topic != "" )
		_has_topic = true;

	//_channel_oper = usr;

	//_members.push_back(usr);
	addMember(usr);
	addOper(usr);
	//_oper.push_back(usr);

	/*string usr_mode = usr->getMode();
	if (usr_mode.find('o') == string::npos)
		usr->setMode(usr_mode + "o");*/
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

vector<User*> const	&Channel::getOper() const {
	return _oper;
}

/*User *				Channel::getOperator( void ) {
	return _channel_oper;
}*/

string const		&Channel::getMode( void ) const {
	return _mode;
}

string const Channel::getCreationDate() const {
	ostringstream s;
	s << fixed << _creation_date;
	string date = s.str();
	return date;
}

string const Channel::getTopicWhen() const {
	ostringstream s;
	s << fixed << _topic_when;
	string date = s.str();
	return date;
}

User *					Channel::getTopicWho( void ) const {
	return _topic_who;
}

size_t 			Channel::getLimit( void ) const {
	return _limit;
}

vector<string> const	&Channel::getBanMask( void ) const {
	return _banned_mask;
}

void    			Channel::setName(string const & name) {
	_name = name;
}

void    			Channel::setKey(string const & key) {
	_key = key;
	_has_key = true;
}

void    			Channel::setTopic(string const & topic, User * u) {
	_topic = topic;
	_has_topic = true;
	_topic_who = u;
	time_t now = time(0);
	_topic_when = (intmax)now;
}

void    			Channel::unsetTopic(User * u) {
	_topic = "";
	_has_topic = false;
	_topic_who = u;
	time_t now = time(0);
	_topic_when = (intmax)now;
}

void    			Channel::unsetKey() {
	_key = "";
	_has_key = false;
}

void				Channel::setMode( string mode ) {
	_mode = mode;
}

void    			Channel::setLimit( int limit ) {

	if ( limit > 0 && limit <= MAX_USR_PER_CHAN )
		_limit = limit;
	else
		_limit = MAX_USR_PER_CHAN;
}

void				Channel::addMember( User * usr ) {
	_members.push_back(usr);
}

void				Channel::deleteMember( User * usr ) {
	
	for ( size_t i = 0; i < this->getNbMembers(); i++ ) {
		if ( usr->getNick() == _members[i]->getNick() ) {
			if ( isOper( *usr ) )
				deleteOper( usr );
			_members.erase(_members.begin() + i);
		}
	}
}

void				Channel::addModerator( User * usr ) {
	_moderators.push_back(usr);
}

void				Channel::deleteModerator( User * usr ) {
	
	for ( size_t i = 0; i < _moderators.size(); i++ ) {
		if ( usr->getNick() == _moderators[i]->getNick() )
			_moderators.erase(_moderators.begin() + i);
	}
}

void				Channel::addOper( User * usr ) {
	_oper.push_back(usr);
}

void				Channel::deleteOper( User * usr ) {
	
	for ( size_t i = 0; i < _oper.size(); i++ ) {
		if ( usr->getNick() == _oper[i]->getNick() )
			_oper.erase(_oper.begin() + i);
	}
}

/*void				Channel::ban( string to_ban, string key ) {

	if ( _banned.find(key) != _banned.end() )
		_banned[key].push_back(to_ban);
}*/

void				Channel::ban( string mask ) {
	_banned_mask.push_back(mask);
}

void				Channel::unban( string mask ) {
	
	for ( size_t i = 0; i < _banned_mask.size(); i++ ) {
		cout << "mask: " << mask << endl;
		cout << "_banned_mask: " << _banned_mask[i] << endl;
		if ( mask == _banned_mask[i] )
			_banned_mask.erase(_banned_mask.begin() + i);
	}
}

bool				Channel::isBanned( User const & usr ) {

	for ( size_t i = 0; i < _banned_mask.size(); i++ ) {
		if ( ft_match(usr.fci(), _banned_mask[i]) )
			return true;
	}
	/*if ( find( _banned["nick"].begin(), _banned["nick"].end(), usr.getNick()) != _banned["nick"].end() )
		return true;
	if ( find( _banned["user"].begin(), _banned["user"].end(), usr.getUsername()) != _banned["user"].end() )
		return true;
	if ( find( _banned["host"].begin(), _banned["host"].end(), usr.getHostname()) != _banned["host"].end() )
		return true;*/
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

bool					Channel::isPrivate( void ) {

	return this->getMode().find("p") != string::npos;
}

bool					Channel::isSecret( void ) {

	return this->getMode().find("s") != string::npos;
}

bool					Channel::isModerated( void ) {

	return this->getMode().find("m") != string::npos;
}

bool					Channel::isTopicSettableByOperOnly( void ) {

	return this->getMode().find("t") != string::npos;
}

string				Channel::getMembersList( void ) {

	string reply;

	for ( size_t i = 0; i < _members.size(); i++ ) {
		if (this->isOper(*_members[i]))
			reply += "@";
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

bool				Channel::isOper( User const & usr ) {
	for ( size_t i = 0; i < _oper.size(); i++ ) {
		if ( usr.getNick() == _oper[i]->getNick())
			return true;
	}
	return false;
}

bool				Channel::isModerator( User const & usr ) {
	for ( size_t i = 0; i < _moderators.size(); i++ ) {
		if ( usr.getNick() == _moderators[i]->getNick())
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
	stream << " created by " << Channel.getOper()[0]->getNick();
	if (Channel.getHasKey())
		stream << " is private (key: " << Channel.getKey() << ")";
	else
		stream << " is public";
	return stream;

}
