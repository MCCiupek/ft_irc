#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "headers.hpp"

// ************************************************************************** //
//                            	Channel Class                                 //
// ************************************************************************** //

class Channel {

	private:

		/*								MEMBERS VARIABLES							*/

		string			_name;
        string			_key;
        bool			_has_key;
        unsigned int	_nb_members;
        string			_topic;
        bool			_has_topic;
        vector<User*>	_members;
        User*			_channel_oper;

		/*								CONSTRUCTORS								*/

		Channel(Channel const& src);

		/*								MEMBERS FUNCTIONS							*/


	public:

		/*								CONSTRUCTORS								*/

		Channel( void );
		Channel(string name);
		virtual ~Channel( void ); 

		Channel & operator=(Channel const& src); 

		/*								GETTERS										*/

		string const 			&getName( void ) const;
		string const 			&getKey( void ) const;
		bool const 				&getHasKey( void ) const;
		unsigned int const 		&getNbMembers( void ) const;
		string const 			&getTopic( void ) const;
		bool const 				&getHasTopic( void ) const;
		vector<User*> const 	&getMembers( void ) const;
		User *					getOperator( void );

		/*								SETTERS										*/

        void    				setName(string const & name);
        void    				setKey(string const & key);
        void    				setTopic(string const & topic);
        void    				unsetTopic();

		/*								MEMBERS FUNCTIONS							*/


};

ostream & operator<<(ostream & stream, Channel &Channel);

#endif
