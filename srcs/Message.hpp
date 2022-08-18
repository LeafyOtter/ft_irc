#pragma once

#include "c_irc.hpp"
#include "Channel.hpp"

#include <string>

namespace c_irc
{
	class Message
	{
		public :

		typedef typename c_irc::users_t::iterator	users_it;

		private:

		std::string		message;
		users_it		first;
		users_it		last;
		bool			to_pop;

		Message();

		public:

		Message(users_it first, users_it last);
		~Message();

		std::string		get_message() const;

		void			set_message(std::string new_message);

		void			prepare_message();
	};
} // namespace c_irc