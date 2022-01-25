#include "headers.hpp"

/*
	Command: KICK
	Parameters: <channel> <user> [<comment>]

	The KICK command can be  used  to  forcibly  remove  a  user  from  a
	channel.   It  'kicks  them  out'  of the channel (forced PART).

		Only a channel operator may kick another user out of a  channel.
	Each  server that  receives  a KICK message checks that it is valid
	(ie the sender is actually a  channel  operator)  before  removing
	the  victim  from  the channel.

	Numeric Replies:

			ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
			ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
			ERR_NOTONCHANNEL

	Examples:

	KICK &Melbourne Matthew         ; Kick Matthew from &Melbourne

	KICK #Finnish John :Speaking English
									; Kick John from #Finnish using
									"Speaking English" as the reason
									(comment).

	:WiZ KICK #Finnish John         ; KICK message from WiZ to remove John
									from channel #Finnish

	NOTE:
		It is possible to extend the KICK command parameters to the
	following:

	<channel>{,<channel>} <user>{,<user>} [<comment>]
*/

void		kick( vector<string> args, User &usr, Server &srv ) {

	vector<string>	chans;
	vector<string>	victims;
	string			reason;
	Channel *		cnl;
	User	*		victim;
	
	if ( args.size() < 2 ) {
		send_error( usr, ERR_NEEDMOREPARAMS, "KICK" );
		return ;
	}

	chans = ft_split(args[0], ",");
	victims = ft_split(args[1], ",");

	reason = ":";
	if ( args.size() > 2 )
		reason = ft_join(args, " ", 2);

	for (size_t i = 0; i < chans.size(); i++) {

		cnl = srv.getChannelByName( chans[i] );

		// if channel doesnt exists
		if ( cnl == NULL ) {
			// TODO: mask?
			send_error( usr, ERR_NOSUCHCHANNEL, chans[i] );
			continue ;
		}

		// if user is not on channel
		if ( !cnl->isOnChann(usr) ) {
			send_error( usr, ERR_NOTONCHANNEL, chans[i] );
			continue ;
		}

		// if user is not channel operator
		if ( !cnl->isOper(usr) ) {
			send_error( usr, ERR_CHANOPRIVSNEEDED, chans[i] );
			continue ;
		}

		for (size_t j = 0; j < victims.size(); j++) {

			victim = srv.getUserByNick(victims[j]);

			// if victim is not a vilid nick
			if ( !victim ) {
				send_error( usr, ERR_NOSUCHNICK, victims[j] );
				continue ;
			}

			// if victim is not on channel
			if ( !cnl->isOnChann(*victim) ) {
				send_error( usr, ERR_NOTONCHANNEL, chans[i] );
				continue ;
			}

			vector<User*> usrs = cnl->getMembers();
			for (size_t k = 0; k < usrs.size(); k++) {
				send_notice(usr, *usrs[k], NTC_KICK(cnl->getName(), victim->getNick(), reason));
			}
		
			victim->deleteChannel(cnl);

			// TODO: PUT THE FOLLOWING CODE IN USER::DELETECHANNEL() >>>>>>>>>>>>>>>>>
			// if channel is user's current channel
			if ( victim->getCurrChan()->getName() == cnl->getName() ) {
				if ( victim->getChannels().size() > 0 )
					victim->setCurrChan(victim->getChannels().back());
				else
					victim->setCurrChan(NULL);
			}
			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

			// Delete chan if usr leaving is the last usr in chan
			if (cnl->getNbMembers() == 0)
				srv.deleteChannel(cnl);
		
		}
	}
}