#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "headers.hpp"

// ************************************************************************** //
//                            	Channel Class                                 //
// ************************************************************************** //

class Server;
class User;

class Channel {

	private:

		/*								MEMBERS VARIABLES							*/

		string							_name;
        string							_key;
        bool							_has_key;
        string							_topic;
        bool							_has_topic;
        vector<User*>					_members;
		vector<User*>					_invited_usrs;
		vector<User*>					_moderators;
		map< string,vector<string> >	_banned;
		vector<string>					_banned_mask;
        //User*							_channel_oper;
		vector<User*>					_oper;
		string							_mode;
		size_t							_limit;
		double							_creation_date;
		double							_topic_when;
		User *							_topic_who;

		/*								CONSTRUCTORS								*/

		Channel(Channel const& src);

		/*								MEMBERS FUNCTIONS							*/


	public:

		/*								CONSTRUCTORS								*/

		Channel( void );
		Channel(string name);
		Channel(string name, string key, string topic, User * usr, string mode = "");
		virtual ~Channel( void ); 

		Channel & operator=(Channel const& src); 

		/*								GETTERS										*/

		string const 			&getName( void ) const;
		string const 			&getKey( void ) const;
		bool const 				&getHasKey( void ) const;
		size_t		 	 		getNbMembers( void ) const;
		string const 			&getTopic( void ) const;
		bool const 				&getHasTopic( void ) const;
		vector<User*> const 	&getMembers( void ) const;
		vector<User*> const		&getOper( void ) const;
		//User *				getOperator( void );
		string const			&getMode( void ) const;
		size_t					getLimit( void ) const;
		string const			getCreationDate( void ) const;
		string const			getTopicWhen( void ) const;
		User *					getTopicWho( void ) const;
		vector<string> const	&getBanMask( void ) const;

		/*								SETTERS										*/

        void    				setName(string const & name);
        void    				setKey(string const & key);
        void    				setTopic(string const & topic, User * u);
        void    				unsetTopic(User * u);
		void    				unsetKey();
		void					setMode( string mode );
		void					setLimit( int limit );

		/*								MEMBERS FUNCTIONS							*/

		void					ban( string mask );
		void					unban( string mask );
		void					addMember( User * usr );
		void					deleteMember( User * usr );
		void					addModerator( User * usr );
		void					deleteModerator( User * usr );
		void					addOper( User * usr );
		void					deleteOper( User * usr );
		bool					isBanned( User const & usr );
		void					invite( User * usr );
		bool					isInvited( User const & usr );
		bool					isInviteOnly( void );
		bool					isPrivate( void );
		bool					isSecret( void );
		bool					isModerated( void );
		bool					isTopicSettableByOperOnly( void );
		string					getMembersList( void );
		bool					isOnChann( User const & usr );
		bool					isOper( User const & usr );
		bool					isModerator( User const & usr );
		string					MembersToString( User const & u, Server const & s );

};

ostream & operator<<(ostream & stream, Channel &Channel);

#endif
