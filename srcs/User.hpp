#pragma once

#include <string>
#include <vector>

#include <poll.h>
#include <stdint.h>

namespace c_irc
{

	typedef std::vector<pollfd>		pollfds_t;

	class User
	{
		private:

		std::string		nick;
		std::string		user;
		std::string		host;
		std::string		realname;

		uint16_t		mode;

		int				fd;

		pollfds_t		&pfds;
		size_t			pfds_index;

		std::string		buffer;

		bool			to_delete;

		std::string		leave_msg;

		User();

		public:

		User(int new_fd, pollfds_t &new_pfds);
		~User();

		std::string		get_nick() const;
		std::string		get_user() const;
		std::string		get_host() const;
		std::string		get_realname() const;
		uint16_t		get_mode() const;
		int				get_fd() const;
		std::string		get_buffer() const;
		std::string		get_leave_msg() const;

		void			set_nick(std::string new_nick);
		void			set_user(std::string new_user);
		void			set_host(std::string new_host);
		void			set_realname(std::string new_realname);
		void			set_mode(uint16_t new_mode);
		void			set_buffer(std::string new_buffer);
		void			append_buffer(std::string append);
		void			set_leave_msg(std::string new_msg);

		bool			is_mode(uint16_t flag) const;
		void			set_flag_mode(uint16_t flag);
		void			unset_flag_mode(uint16_t flag);

		void			set_delete();
		bool			is_delete() const;

		void			set_pollout();
		void			unset_pollout();
		pollfd			*get_pollfd();
	};

	std::ostream& operator<<(std::ostream &os, const User &user);
} // namespace c_irc