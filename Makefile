# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmanetti <fmanetti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/03 19:40:40 by fmanetti          #+#    #+#              #
#    Updated: 2022/01/26 00:26:38 by fmanetti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				=		clang++

FLAGS			=		-Wall -Wextra -Werror -std=c++98
FSANITIZE		=		-fsanitize=address -g3

RM				=		rm -rf

DIR_HEADERS		=		./includes/

HEADER			=		colors.hpp		\
						errors.hpp		\
						headers.hpp		\
						parsing.hpp		\
						Server.hpp		\
						User.hpp		\
						utils.hpp		\
						cmd.hpp

HEADERS			=		$(addprefix $(DIR_HEADERS), $(HEADER))

DIR_SRCS		=		./srcs/

SRC				=		main.cpp		\
						errors.cpp		\
						utils.cpp		\
						parsing.cpp		\
						User.cpp		\
						Server.cpp		\
						Channel.cpp		\
						cmd/nick.cpp	\
						cmd/user.cpp	\
						cmd/ping.cpp	\
						cmd/pong.cpp	\
						cmd/who.cpp		\
						cmd/mode.cpp	\
						cmd/join.cpp	\
						cmd/privmsg.cpp	\
						cmd/part.cpp	\
						cmd/topic.cpp	\
						cmd/names.cpp	\
						cmd/quit.cpp	\
						cmd/kick.cpp	\
						cmd/notice.cpp	\
						cmd/invite.cpp	\
						cmd/pass.cpp	\
						cmd/oper.cpp	\
						conf.cpp

SRCS			=		$(addprefix $(DIR_SRCS), $(SRC))

OBJS			=		$(SRCS:.cpp=.o)

NAME			=		ircserv

UNAME			:=		$(shell uname)

ifeq ($(UNAME),Darwin)
	OS = -D MACOS
endif

ifeq ($(UNAME),Linux)
	OS = -D LINUX
endif

all:			$(NAME)

$(NAME) :		echoCL $(OBJS) $(HEADERS) echoOK echoCS 
				$(CC) $(FLAGS) $(FSANITIZE) $(OS) $(OBJS) -o $(NAME)

%.o: %.cpp
				$(CC) $(FLAGS) $(FSANITIZE) $(OS) -I $(DIR_HEADERS) -c $< -o $@
				printf "$(GREEN)██"

norme:			fclean
				norminette $(DIR_SRCS)
				norminette $(DIR_HEADERS)

clean:			echoCLEAN
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

git:			fclean
				git pull
				git add .
				git commit -m "$(shell date)"
				git push

re:				fclean all

.PHONY:			all, clean, fclean, re, norme, git, bonus

.SILENT:

# COLOR
#----------------reset----------------#
NC = \033[0m

#-----------Regular Colors------------#
BLACK = \033[0;30m
RED = \033[0;31m
GREEN = \033[32m
YELLOW = \033[33;33m
BLUE = \033[0;34m
PURPLE = \033[1;35m
CYAN = \033[1;36m
WHITE = \033[0;37m

###########################ECHO
echoCL:
	printf "\n$(YELLOW)===> Compiling $(RED)$(NAME)$(NC)\n"
echoOK:
	printf "$(GREEN) OK$(NC)\n"
echoCS :
	printf "$(GREEN)===> Compilation Success$(NC)\n"
echoCLEAN :
	printf "$(PURPLE)$(NAME) ===> Cleaning$(NC)\n"
echoFCLEAN :
	printf "$(PURPLE)$(NAME) ===> Cleaning Exec & Lib$(NC)\n"