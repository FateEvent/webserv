NAME		=	webserv
CC			=	c++
FLAGS		=	-Wall -Werror -Wextra -std=c++11
INCLUDES	=	./include/common.h \
				./include/Webserv.hpp \
				./include/Client.hpp \
				./include/search.hpp
FILES		=	Webserv.cpp \
				Client.cpp \
				search.cpp \
				parse.cpp \
				main.cpp
RM			=	rm -rf
SRCS		=	$(addprefix ./src/, $(FILES))
OBJS		=	$(SRCS:%.cpp=%.o)

$(NAME)		:	$(OBJS)
				$(CC) $(OBJS) -o $(NAME)

.cpp.o		:
	$(CC) $(FLAGS) -c $< -o ${<:.cpp=.o}

all			:	$(NAME)

clean:
	$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re