CC		=		clang++

FLAGS	=		-Wall -Wextra -Werror -std=c++98

RM =			rm -rf

DIR_HEADERS =	./includes/

DIR_SRCS =		./srcs/

SRC =			main.cpp \
				errors.cpp \
				utils.cpp

SRCS =			$(addprefix $(DIR_SRCS), $(SRC))

OBJS =			$(SRCS:.cpp=.o)

NAME =			ircserv

UNAME := 		$(shell uname)

ifeq ($(UNAME),Darwin)
	OS = -D MACOS
endif

ifeq ($(UNAME),Linux)
	OS = -D LINUX
endif

all:			$(NAME)

$(NAME) :		echoCL $(OBJS) echoOK echoCS
				$(CC) $(FLAGS) $(OS) $(OBJS) -o $(NAME)

%.o: %.cpp
				$(CC) $(FLAGS) $(OS) -I $(DIR_HEADERS) -c $< -o $@
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
	printf "$(PURPLE)$(NAME) ===> Cleanning$(NC)\n"
echoFCLEAN :
	printf "$(PURPLE)$(NAME) ===> Cleanning Exec & Lib$(NC)\n"