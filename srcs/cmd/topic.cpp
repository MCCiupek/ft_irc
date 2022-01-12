#include "headers.hpp"

/*
	Command: TOPIC
	Parameters: <channel> [ <topic> ]

	The TOPIC command is used to change or view the topic of a channel.
	The topic for channel <channel> is returned if there is no <topic>
	given.  If the <topic> parameter is present, the topic for that
	channel will be changed, if this action is allowed for the user
	requesting it.  If the <topic> parameter is an empty string, the
	topic for that channel will be removed.

	Numeric Replies:

			ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
			RPL_NOTOPIC                     RPL_TOPIC
			ERR_CHANOPRIVSNEEDED

	Examples:

		:WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the
                                   topic.

		TOPIC #test :another topic      ; Command to set the topic on #test
                                   		to "another topic".

		TOPIC #test :                   ; Command to clear the topic on
                                   		#test.

		TOPIC #test                     ; Command to check the topic for
                                   			#test.
*/

void		topic( vector<string> args, User &usr, Server &srv ) {

	Channel *		cnl;

	if ( args.size() < 2 )
		return send_error( usr, ERR_NEEDMOREPARAMS, args[0] );

	if ( *(args[args.size() - 1].end() - 1) == '\n')
		args[args.size() - 1] = args[args.size() - 1].substr(0, args[args.size() - 1].length()-1);

	cnl = srv.getChannelByName( args[1] );
	if ( cnl == NULL || !cnl->isOnChann(usr) )
		return send_error( usr, ERR_NOTONCHANNEL, args[1] );
	
	if ( args.size() > 2 ) {
		if ( cnl->isTopicSettableByOperOnly() && !cnl->isOper(usr) ) {}
			//return send_error( usr, ERR_CHANOPRIVSNEEDED, args[1] );
		if ( args[2] == ":" ) {
			cnl->unsetTopic();
		} else {
			string topic = ft_join( args, " ", 2);
			if ( topic[0] == ':' )
				topic = &topic[1];
			cnl->setTopic( topic );
		}
	}
	if ( cnl->getHasTopic() )
		send_reply(usr, 332, RPL_TOPIC(cnl->getName(), cnl->getTopic()));
	else
		send_reply(usr, 331, RPL_NOTOPIC(cnl->getName()));
}