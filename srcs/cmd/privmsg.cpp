#include "headers.hpp"

/*
	Command: PRIVMSG
	Parameters: <receiver>{,<receiver>} <text to be sent>

	PRIVMSG is used to send private messages between users.  <receiver>
	is the nickname of the receiver of the message.  <receiver> can also
	be a list of names or channels separated with commas.

	The <receiver> parameter may also be a host mask  (#mask)  or  server
	mask  ($mask).   In  both cases the server will only send the PRIVMSG
	to those who have a server or host matching the mask.  The mask  must
	have at  least  1  (one)  "."  in it and no wildcards following the
	last ".".  This requirement exists to prevent people sending messages
	to  "#*"  or "$*",  which  would  broadcast  to  all  users; from
	experience, this is abused more than used responsibly and properly.
	Wildcards are  the  '*' and  '?'   characters.   This  extension  to
	the PRIVMSG command is only available to Operators.

	Numeric Replies:

		ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
		ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
		ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
		ERR_NOSUCHNICK
		RPL_AWAY
*/

void		send_to_all_in_chan( Channel * Chan, string txt, User &usr ) {
	
	vector<User*> users = Chan->getMembers();
	for ( size_t i = 0; i < users.size(); i++ ) {
		if (users[i]->getNick() != usr.getNick()) {
			send_notice(usr, *users[i], NTC_PRIVMSG(Chan->getName(), txt));
		}
	}
}

void		send_privmsg_to_usr( string recv, string txt, User &usr, Server &srv ) {
	
	User *	receiver;

	receiver = srv.getUserByNick(recv);
	if ( !receiver )
		return send_error(usr, ERR_NOSUCHNICK, recv);
	send_notice(usr, *receiver, NTC_PRIVMSG(receiver->getNick(), txt));
}

void		send_privmsg_to_chan( string recv, string txt, User &usr, Server &srv ) {
	
	Channel * channel;
	string	msg;

	channel = srv.getChannelByName(recv);
	if ( !channel )
		return send_error(usr, ERR_NOSUCHCHANNEL, recv);
	if (!(channel->isOnChann(usr) || channel->isOper(usr)) )
		return send_error(usr, ERR_CANNOTSENDTOCHAN, recv);
	if ( channel->isModerated() && !channel->isModerator(usr) )
		return send_error(usr, ERR_CANNOTSENDTOCHAN, recv);
	send_to_all_in_chan( channel, txt, usr );
}

void		send_privmsg( string recv, string txt, User &usr, Server &srv ) {

	string 	mask = "#";
	
	if ( txt.length() == 0 )
		return send_error(usr, ERR_NOTEXTTOSEND, recv);
	if ( recv.length() == 0 )
		return send_error(usr, ERR_NORECIPIENT, recv);
	if ( mask.find(recv[0], 0) == string::npos )
		send_privmsg_to_usr(recv, txt, usr, srv);
	else
		send_privmsg_to_chan(recv, txt, usr, srv);
	return;
}

void		privmsg( vector<string> args, User &usr, Server &srv ) {

	if (args.size() < 1)
		return send_error(usr, ERR_NORECIPIENT, "");
	if (args.size() < 2)
		return send_error(usr, ERR_NOTEXTTOSEND, "");

	vector<string> recvs = ft_split(args[0], ",");

	if (has_duplicates(recvs))
		return send_error(usr, ERR_TOOMANYTARGETS, find_duplicates(recvs));

	for (vector<string>::iterator it = recvs.begin(); it != recvs.end(); it++)
		send_privmsg(*it, ft_join(args, " ", 1), usr, srv);
}