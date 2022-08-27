NAME = ircserv

C++ = c++

FSAN = -fsanitize=address -fsanitize=leak -fsanitize=undefined

FLAG = -Wall -Werror -Wextra -std=c++98 -g $(FSAN)
INCS = -I./srcs -MMD -MP

SRCS_LIST = \
	main.cpp \
	Server.cpp \
	User.cpp \
	Message.cpp \
	Channel.cpp \
	Command.cpp \
	commands/user/nick.cpp \
	commands/user/pass.cpp \
	commands/user/user.cpp \
	commands/user/cap.cpp \
	commands/utils_cmd.cpp \
	commands/chan/invite.cpp \
	commands/chan/join.cpp \
	commands/chan/kick.cpp \
	commands/chan/list.cpp \
	commands/chan/names.cpp \
	commands/chan/part.cpp \
	commands/chan/topic.cpp 


SRCS_DIR := srcs
OBJS_DIR := objs

SRCS = $(addprefix $(SRCS_DIR)/,$(SRCS_LIST))
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

all : $(NAME)

$(NAME)	: $(OBJS) 
	$(C++) $(FLAG) $(INCS) $(OBJS) -o $@

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.cpp
	@mkdir -pv $(dir $@)
	$(C++) $(FLAG) $(INCS) -c $< -o $@

clean :
	@if [ -d $(OBJS_DIR) ]; \
		then \
		rm -rfv $(OBJS_DIR); \
		fi;

fclean : clean
	@if [ -e $(NAME) ]; \
		then \
		rm -v $(NAME); \
		fi;

re : fclean all

.PHONY : all clean fclean re

-include $(DEPS)