#include "headers.hpp"

Channel::Channel( void ) {

}

Channel::Channel(string name) : 
		_name(name),
		_key(""),
		_has_key(false),
		_nb_members(0),
		_topic(""),
		_has_topic(false),
		_mode("")
{

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

unsigned int const	&Channel::getNbMembers() const {
	return _nb_members;
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
}

void    			Channel::setTopic(string const & topic) {
	_topic = topic;
}

void    			Channel::unsetTopic() {
	_topic = "";
}

void				Channel::setMode( string mode ) {
	_mode = mode;
}

ostream & operator<<(ostream & stream, Channel &Channel) {

	stream << "Channel: " << Channel.getName() << endl;
	return stream;
}
