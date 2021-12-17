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
		map< string,vector<string> >	_banned;
        User*							_channel_oper;
		string							_mode;

		/*								CONSTRUCTORS								*/

		Channel(Channel const& src);

		/*								MEMBERS FUNCTIONS							*/


	public:

		/*								CONSTRUCTORS								*/

		Channel( void );
		Channel(string name);
		Channel(string name, string key, string topic, User * usr);
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
		User *					getOperator( void );
		string const			&getMode( void ) const;

		/*								SETTERS										*/

        void    				setName(string const & name);
        void    				setKey(string const & key);
        void    				setTopic(string const & topic);
        void    				unsetTopic();
		void					setMode( string mode );

		/*								MEMBERS FUNCTIONS							*/

		void					addMember( User * usr );
		void					deleteMember( User * usr );
		void					ban( string to_ban, string key );
		bool					isBanned( User const & usr );
		void					invite( User * usr );
		bool					isInvited( User const & usr );
		bool					isInviteOnly( void );
		string					getMembersList( void );
		bool					isOnChann( User const & usr );

};

ostream & operator<<(ostream & stream, Channel &Channel);

#endif
