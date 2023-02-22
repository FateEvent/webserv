NAME		=	webserv
CC			=	c++
FLAGS		=	-Wall -Werror -Wextra -std=c++11 -g3
INCLUDES	=	./include/search.hpp \
				./include/common.h \
				./include/Webserv.hpp
FILES		=	Webserv.cpp \
				Client.cpp \
				search.cpp \
				main.cpp
RM			=	rm -rf
SRCS		=	$(addprefix ./src/, $(FILES))
OBJS		=	$(SRCS:%.c=%.o)

$(NAME)		:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)

all			:	$(NAME)

clean		:	$(RM) $(OBJS)

flcean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re