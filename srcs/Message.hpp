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

		bool				to_pop;
		std::string			message;
		target_type_t		target_type;
		int					target;
		chan_users_it_t		first_target;
		chan_users_it_t		last_target;
		chan_users_it_t		sender;

		serv_users_t		&users;

		Message();

		public:

		Message(serv_users_t &u, chan_users_it_t first, chan_users_it_t last);
		Message(serv_users_t &u, int fd, std::string msg);
		Message(const Message &other);
		~Message();

		Message &operator=(const Message &other);

		std::string		get_message() const;
		bool			get_status();

		void			set_message(std::string new_message);
		void			set_sender(chan_users_it_t new_sender);
		void			set_status();

		int				nb_users() const;


		void			prepare();
	};
} // namespace c_irc