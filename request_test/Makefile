NAME		=	serv1
CC			=	c++
FLAGS		=	-std=c++98
INCLUDES	=	./request.hpp
FILES		=	serv1.cpp \
				request.cpp
RM			=	rm -rf
SRCS		=	$(addprefix ./, $(FILES))
OBJS		=	$(SRCS:%.c=%.o)

$(NAME)		:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)

all			:	$(NAME)

clean		:	$(RM) $(OBJS)

flcean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re