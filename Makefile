# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stissera <stissera@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 10:20:15 by stissera          #+#    #+#              #
#    Updated: 2023/03/13 20:54:52 by stissera         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
CC			=	c++
FLAGS		=	-Wall -Werror -Wextra -std=c++11 -g
INCLUDES	=	./includes/utils.h
FILES		=	utils/s_config.cpp \
				utils/file_to_map.cpp \
				utils/multi_to_config.cpp \
				utils/parse_err_page.cpp \
				utils/s_header.cpp \
				Client.cpp \
				Webserv.cpp \
				main.cpp
RM			=	rm -rf
SRCS		=	$(addprefix ./srcs/, $(FILES))
OBJS		=	$(SRCS:%.c=%.o)

$(NAME)		:	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)

all			:	$(NAME)

clean		:	$(RM) $(OBJS)

flcean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re