#pragma once

#include <string>
#include "c_irc.hpp"

namespace c_irc
{
	class User
	{
		private:

		std::string		nick;
		std::string		user;
		std::string		host;
		std::string		realname;

		uint16_t		mode;

		pollfd			*pfd;

		User();

		public:

		User(pollfd *new_pfd);
		~User();

		std::string		get_nick() const;
		std::string		get_user() const;
		std::string		get_host() const;
		std::string		get_realname() const;
		uint16_t		get_mode() const;
		pollfd			*get_pfd() const;

		void			set_nick(std::string new_nick);
		void			set_user(std::string new_user);
		void			set_host(std::string new_host);
		void			set_realname(std::string new_realname);
		void			set_mode(uint16_t new_mode);
	};
} // namespace c_irc
