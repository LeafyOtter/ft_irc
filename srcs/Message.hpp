#pragma once

#include "c_irc.hpp"
#include "Channel.hpp"

#include <string>

namespace c_irc
{
	class Message
	{
		private:	

		typedef enum target_type_e {
			TARGET_UNREGISTERED,
			TARGET_RANGE
		} target_type_t;

		bool			to_pop;
		std::string		message;
		target_type_t	target_type;
		pollfd			*target;
		users_it_t		first_target;
		users_it_t		last_target;
		users_it_t		sender;

		Message();

		public:

		Message(users_it_t first, users_it_t last);
		Message(std::string msg, users_it_t first, users_it_t last);
		Message(std::string msg, pollfd *new_pfd);
		Message(const Message &other);
		~Message();

		Message &operator=(const Message &other);

		std::string		get_message() const;

		void			set_status();
		bool			get_status();

		int				nb_users() const;

		void			set_sender(users_it_t new_sender);

		void			set_message(std::string new_message);

		void			prepare();
	};
} // namespace c_irc