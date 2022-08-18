#pragma once

#include "c_irc.hpp"
#include "Channel.hpp"

#include <string>

namespace c_irc
{
	class Message
	{
		private:	

		typedef typename users_set_t::iterator users_it;

		std::string		message;
		users_it		first;
		users_it		last;
		users_it		sender;
		bool			to_pop;

		Message();

		public:

		Message(users_it first, users_it last);
		Message(std::string msg, users_it first, users_it last);
		~Message();

		std::string		get_message() const;

		void			set_status();
		bool			get_status();

		int				nb_users() const;

		void			set_sender(users_it new_sender);

		void			set_message(std::string new_message);

		void			prepare();
	};
} // namespace c_irc