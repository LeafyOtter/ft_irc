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
			TARGET_USER,
			TARGET_CHANNEL
		} target_type_t;

		bool				to_pop;
		int					utarget;
		c_irc::Channel		*ctarget;
		target_type_t		target_type;
		int					sender;
		std::string			message;
		
		serv_users_t		&users;

		Message();

		public:

		Message(serv_users_t &u, std::string msg, int target_fd);
		Message(serv_users_t &u, std::string msg, c_irc::Channel *c, int sender_fd);
		Message(const Message &other);
		~Message();

		Message &operator=(const Message &other);

		std::string		get_message() const;
		bool			get_status();

		void			set_message(std::string new_message);
		void			set_sender(int new_sender);
		void			set_status();

		void			append_message(std::string new_message);

		int				nb_targets() const;

		void			prepare();
	};
} // namespace c_irc