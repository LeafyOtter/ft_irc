#pragma once

#include "c_irc.hpp"
#include "User.hpp"

#include <map>
#include <string>

namespace c_irc
{
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

		list_users_t	ban_list;
		list_users_t	invite_list;

		chan_users_t	chan_users;

		serv_users_t	&serv_users;

		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

		Channel();
		
		public:

		Channel(serv_users_t &su, std::string new_name, int fd);
		~Channel();

		std::string		get_name() const;
		std::string		get_topic() const;
		std::string		get_key() const;
		uint16_t		get_mode() const;
		uint16_t		get_limit() const;
		size_t			get_number_of_users() const; 				

		void			set_topic(std::string new_topic);
		void			set_key(std::string new_key);
		void			set_mode(uint16_t new_mode);
		void			unset_mode(uint16_t new_mode);
		void			set_limit(uint16_t new_limit);

		void			set_user_mode(int fd, uint16_t new_mode);
		void			unset_user_mode(int fd, uint16_t new_mode);
		bool			is_mode(uint16_t mode) const;

		void			set_user_mode(int fd);
		void			unset_user_mode(int fd);
		bool			is_user_op(int fd);

		void			add_user(int fd);
		void			remove_user(int fd);

		void			ban_user(std::string new_user);
		void			unban_user(std::string new_user);

		void			invite_user(std::string new_user);
		void			uninvite_user(std::string new_user);

		//bool			is_name_valid(std::string new_name);
		bool			is_user_banned(std::string new_user);
		bool			is_user_invited(std::string new_user);
		bool			is_user_in_channel(std::string new_user);
		bool			is_mode(uint16_t fl);

		int				fd_from_nick(std::string new_nick);

		bool			is_empty() const;

		bool			is_full() const;

		chan_users_it_t		begin();
		chan_users_it_t		end();

		chan_users_it_t		get_user(int fd);
	};
} // namespace c_irc
