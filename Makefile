NAME		=	webserv
CC			=	c++
FLAGS		=	-Wall -Werror -Wextra -std=c++11
INCLUDES	=	./include/common.h \
				./include/Webserv.hpp \
				./include/Client.hpp
FILES		=	Webserv.cpp \
				Client.cpp \
				main.cpp
RM			=	rm -rf
SRCS		=	$(addprefix ./src/, $(FILES))
OBJS		=	$(SRCS:%.cpp=%.o)

$(NAME)		:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)

all			:	$(NAME)

clean:
	$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re