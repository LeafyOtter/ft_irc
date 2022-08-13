#pragma once

#include "c_irc.hpp"

#include <map>
#include <string>

namespace c_irc
{
	typedef users_t::iterator			users_t_it;

	class Channel
	{
		private:

		// max length : 50 chars
		// case insensitive
		// start with "#, &, + or !"
		// no spaces, ^G or commas
		std::string		name;

		std::string		topic;
		std::string		key;
		uint16_t		mode;
		uint16_t		limit;

		users_t			users;

		Channel();

		public:

		Channel(std::string	new_name);
		~Channel();

		std::string		get_name() const;
		std::string		get_topic() const;
		std::string		get_key() const;
		uint16_t		get_mode() const;
		uint16_t		get_limit() const;

		void			set_topic(std::string new_topic);
		void			set_key(std::string new_key);
		void			set_mode(uint16_t new_mode);
		void			set_limit(uint16_t new_limit);

		void			add_user(c_irc::User* new_user);
		void			remove_user(c_irc::User* new_user);

		void			ban_user(std::string new_user);
		void			unban_user(std::string new_user);

		bool			is_name_valid(std::string new_name) const;
	};
} // namespace c_irc
