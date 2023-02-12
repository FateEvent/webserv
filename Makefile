NAME		=	webserv
CC			=	c++
FLAGS		=	-wall -werror -std=c++98
INCLUDES	=	./include/Webserv.hpp
FILES		=	main.cpp \
				Webserv.cpp
RM			=	rm -rf
SRCS		=	$(addprefix ./src/, $(FILES))
OBJS		=	$(SRCS:%.c=%.o)

$(NAME)		:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) $(INCLUDES) -o $(NAME)

all			:	$(NAME)

clean		:	$(RM) $(OBJS)

flcean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re