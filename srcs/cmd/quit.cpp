#include "headers.hpp"

/*
	Syntax:
		QUIT [<message>]

	Parameters:

		The message to advertise.

	Description:

		Terminates the application and advertises the given message on all the
		networks you are connected to.

	Examples:

		/QUIT
		/QUIT <@cartman> Screw you guys, I'm going home!
*/

/*
	Command: QUIT
	Parameters: [<Quit message>]

	A client session is ended with a quit message.  The server must close
	the connection to a client which sends a QUIT message. If a "Quit
	Message" is given, this will be sent instead of the default message,
	the nickname.

	When netsplits (disconnecting of two servers) occur, the quit message
	is composed of the names of two servers involved, separated by a
	space.  The first name is that of the server which is still connected
	and the second name is that of the server that has become
	disconnected.

	If, for some other reason, a client connection is closed without  the
	client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs
	on socket), the server is required to fill in the quit  message  with
	some sort  of  message  reflecting the nature of the event which
	caused it to happen.

	Numeric Replies:

			None.

	Examples:

	QUIT :Gone to have lunch        ; Preferred message format
*/

void		quit( vector<string> args, User &usr, Server &srv )
{
	string	msg;

	if (args.size() > 0)
		args[0] = &args[0][1];

	msg = ft_join(args, " ", 0);

	vector<Channel *>	usr_channels = usr.getChannels();

	if (usr_channels.size())
		for (vector<Channel*>::iterator it = usr_channels.begin(); it != usr_channels.end(); it++)
			send_notice_channel(usr, *it, NTC_QUIT(msg));

	usr.leaveAllChans();

	vector<Channel *>	srv_channels = srv.getChannels();

	if (srv_channels.size())
		for (vector<Channel*>::iterator it = srv_channels.begin(); it != srv_channels.end(); it++)
			if ( !(*it)->getNbMembers() )
				srv.deleteChannel(*it);

	int fd = usr.getFd();

	
	srv.del_from_pfds(fd);
	srv.deleteUser( &usr );
	
	cout << BOLDWHITE << "❌ Client #" << fd << " gone away" << RESET << endl;
}