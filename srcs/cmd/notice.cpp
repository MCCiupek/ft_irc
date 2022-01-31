#include "headers.hpp"

/*
	Command: NOTICE
	Parameters: <nickname> <text>

	The NOTICE message is used similarly to PRIVMSG.  The difference
	between NOTICE and PRIVMSG is that automatic replies must never be
	sent in response to a NOTICE message.  This rule applies to servers
	too - they must not send any error reply back to the client on
	receipt of a notice.  The object of this rule is to avoid loops
	between a client automatically sending something in response to
	something it received.  This is typically used by automatons (clients
	with either an AI or other interactive program controlling their
	actions) which are always seen to be replying lest they end up in a
	loop with another automaton.

	See PRIVMSG for more details on replies and examples.
*/

void		send_notice_to_all_in_chan( Channel * Chan, string txt, User &usr ) {
	
	vector<User*> users = Chan->getMembers();
	for ( size_t i = 0; i < users.size(); i++ ) {
		if (users[i]->getNick() != usr.getNick()) {
			send_notice(usr, *users[i], NTC_NOTICE(Chan->getName(), txt));
		}
	}
}

void		send_notice_to_usr( string recv, string txt, User &usr, Server &srv ) {
	
	User *	receiver;

	receiver = srv.getUserByNick(recv);
	if ( !receiver )
		return send_error(usr, ERR_NOSUCHNICK, recv);
	send_notice(usr, *receiver, NTC_NOTICE(receiver->getNick(), txt));
}

void		send_notice_to_chan( string recv, string txt, User &usr, Server &srv ) {
	
	Channel * channel;
	string	msg;

	channel = srv.getChannelByName(recv);
	if ( !channel )
		return send_error(usr, ERR_NOSUCHCHANNEL, recv);
	if (!(channel->isOnChann(usr) || channel->isOper(usr)) )
		return send_error(usr, ERR_CANNOTSENDTOCHAN, recv);
	if ( channel->isModerated() && !channel->isModerator(usr) )
		return send_error(usr, ERR_CANNOTSENDTOCHAN, recv);
	send_notice_to_all_in_chan( channel, txt, usr );
}

void		send_notice( string recv, string txt, User &usr, Server &srv ) {

	string 	mask = "#";
	
	if ( txt.length() == 0 )
		return send_error(usr, ERR_NOTEXTTOSEND, recv);
	if ( recv.length() == 0 )
		return send_error(usr, ERR_NORECIPIENT, recv);
	if ( mask.find(recv[0], 0) == string::npos )
		send_notice_to_usr(recv, txt, usr, srv);
	else
		send_notice_to_chan(recv, txt, usr, srv);
	return;
}

void		notice( vector<string> args, User &usr, Server &srv ) {

	if (args.size() < 1)
		return send_error(usr, ERR_NORECIPIENT, "");
	if (args.size() < 2)
		return send_error(usr, ERR_NOTEXTTOSEND, "");

	vector<string> recvs = ft_split(args[0], ",");

	if (has_duplicates(recvs))
		return send_error(usr, ERR_TOOMANYTARGETS, find_duplicates(recvs));

	for (vector<string>::iterator it = recvs.begin(); it != recvs.end(); it++)
		send_notice(*it, ft_join(args, " ", 1), usr, srv);
}