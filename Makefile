# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stissera <stissera@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 10:20:15 by stissera          #+#    #+#              #
#    Updated: 2023/04/13 17:03:05 by stissera         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
CC			=	c++
FLAGS		=	-Wall -Werror -Wextra -std=c++11 -Og -g -fsanitize=address
INCLUDES	=	./includes/utils.h
FILES		=	utils/file_to_map.cpp \
				utils/multi_to_config.cpp \
				utils/parse_err_page.cpp \
				utils/parse_header.cpp \
				utils/s_clt_data.cpp \
				utils/s_config.cpp \
				utils/s_header.cpp \
				utils/split_to_map.cpp \
				utils/test_path.cpp \
				utils/vector_to_tab.cpp \
				cgi.cpp \
				multipart.cpp \
				Client.cpp \
				get.cpp \
				post.cpp \
				delete.cpp \
				get_page_error.cpp \
				location.cpp \
				main.cpp \
				make_header.cpp \
				Webserv.cpp
RM			=	rm -rf
SRCS		=	$(addprefix ./srcs/, $(FILES))
OBJS		=	$(SRCS:%.cpp=%.o)

$(NAME)		:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)

.cpp.o		:
	$(CC) $(FLAGS) -c $< -o ${<:.cpp=.o}

all			:	$(NAME)

clean:
	$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re