#include "headers.hpp"

Channel::Channel( void ) {

}

Channel::Channel(string name) : 
		_name(name),
		_key(""),
		_has_key(false),
		_nb_members(0),
		_topic(""),
		_has_topic(false)
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
	}
	return *this;
}

string const & Channel::getName() const {
	return _name;
}

string const & Channel::getKey() const {
	if (_has_key)
		return _key;
	return "";
}

bool const & Channel::getHasKey() const {
	return _has_key;
}

unsigned int const & Channel::getNbMembers() const {
	return _nb_members;
}

string const & Channel::getTopic() const {
	if (_has_topic)
		return _topic;
	return "";
}

bool const & Channel::getHasTopic() const {
	return _has_topic;
}

vector<User*> const &Channel::getMembers() const {
	return _members;
}

User *					Channel::getOperator( void ) {
	return _channel_oper;
}

void    				Channel::setName(string const & name) {
	_name = name;
}

void    				Channel::setKey(string const & key) {
	_key = key;
}

void    				Channel::setTopic(string const & topic) {
	_topic = topic;
}

void    				Channel::unsetTopic() {
	_topic = "";
}

ostream & operator<<(ostream & stream, Channel &Channel) {

	stream << "Channel: " << Channel.getName() << endl;
	return stream;
}
