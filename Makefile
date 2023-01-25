
# **************************************************************************** #
# COLORS

GREEN		:= \033[0;92m
YELLOW		:= \033[0;93m
BLUE		:= \033[0;94m
END_COLOR	:= \033[0;39m

# **************************************************************************** #
# VARIABLES

SRCS	=	src/ft_atoi.c \
			src/ft_isdigit.c \
			src/list_utils.c \
			src/routine_utils.c \
			src/time_utils.c \
			src/setups.c \
			src/arg_check.c \
			src/ending.c \
			src/main.c
OBJ		=	${SRCS:.c=.o}
HEADER	=   include/philosophers.h
NAME	=	philo
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -O0

# **************************************************************************** #
# RULES

%.o:	%.c
	@${CC} ${CFLAGS} -c -g $< -o $@
	@echo "$(BLUE)Compiling $<...$(END_COLOR)"

$(NAME):	${OBJ} Makefile
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lpthread
	@echo "$(GREEN)Compiled $(NAME) :)$(END_COLOR)"

all:	${NAME}

clean:
	@rm -f ${OBJ}
	@echo "$(YELLOW)Removed object files.$(END_COLOR)"

fclean: clean
	@rm -f ${NAME}
	@echo "$(YELLOW)Cleaned libraries and $(NAME) executable.$(END_COLOR)"

re:		fclean all
	@echo "$(GREEN)Cleaned all and rebuilt $(NAME) and $(LIB)!$(END_COLOR)"

.PHONY:	all clean fclean re
