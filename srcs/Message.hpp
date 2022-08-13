#pragma once

#include "c_irc.hpp"

#include <string>

namespace c_irc
{
	template <typename iterator>
	class Message
	{
		private:

		std::string		message;
		iterator		first;
		iterator		last;
		bool			to_pop;

		Message();

		public:

		Message(iterator first, iterator last);
		~Message();

		std::string		get_message() const;

		void			set_message(std::string new_message);

		void			prepare_message();
	};
} // namespace c_irc