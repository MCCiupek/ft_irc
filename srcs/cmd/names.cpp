#include "headers.hpp"

/*
	Command: NAMES
	Parameters: [<channel>{,<channel>}]

	By using the NAMES command, a user can list all nicknames that are
	visible to them on any channel that they can see.  Channel names
	which they can see are those which aren't private (+p) or secret (+s)
	or those which they are actually on.  The <channel> parameter
	specifies which channel(s) to return information about if valid.
	There is no error reply for bad channel names.

	If no <channel> parameter is given, a list of all channels and their
	occupants is returned.  At the end of this list, a list of users who
	are visible but either not on any channel or not on a visible channel
	are listed as being on `channel' "*".

	Numerics:

			  RPL_NAMREPLY						  RPL_ENDOFNAMES

	Examples:

	NAMES #twilight_zone,#42		; list visible users on #twilight_zone
									  and #42 if the channels are visible to
									  you.

	NAMES							; list all visible channels and users
*/

bool		cnl_is_visible_to_usr( Channel * cnl, User &usr ) {

	if (cnl->isOnChann(usr))
		return true;
	if (cnl->isPrivate() || cnl->isSecret())
		return false;
	return true;
}

void		print_other_names( User &usr, Server &srv ) {
	
	vector<string>		names;
	map<int, User>		users = srv.getUsers();
	vector<Channel*>	chans;
	string				chan_name = "*";

	for (map<int, User>::iterator it = users.begin(); it != users.end(); it++) {
		if ((it->second).isVisible()) {
			chans = (it->second).getChans();
			for (size_t j = 0; j < chans.size(); j++) {
				if ( cnl_is_visible_to_usr(chans[j], usr) )
					break ;
			}
			if (j == chans.size())
				names.push_back( (it->second).getNick() );
		}
	}
	if (names.size() > 0) {
		send_reply(usr, 353, RPL_NAMREPLY(chan_name, ft_join(names, " ", 0)));
		send_reply(usr, 366, RPL_ENDOFNAMES(chan_name));
	}
}

void		print_chan_names( string channel, User &usr, Server &srv ){
	
	Channel *		cnl;

	cnl = srv.getChannelByName( channel );
	if ( cnl == NULL )
		return ;
	if ( !cnl->isOnChann(usr) && (cnl->isPrivate() || cnl->isSecret()) )
		return ;
	send_reply(usr, 353, RPL_NAMREPLY(cnl->getName(), cnl->getMembersList()));
	send_reply(usr, 366, RPL_ENDOFNAMES(cnl->getName()));
}

void		names( vector<string> args, User &usr, Server &srv ) {

	vector<string>	chans;

	if ( args.size() < 2 )
		chans = srv.getChannelsNames();
	else
		chans = ft_split(args[1], ",");

	for (size_t i = 0; i < chans.size(); i++)
		print_chan_names(chans[i], usr, srv);
	
	if ( args.size() < 2 )
		print_other_names(usr, srv);
}