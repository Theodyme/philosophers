
# **************************************************************************** #
# COLORS

GREEN		:= \033[0;92m
YELLOW		:= \033[0;93m
BLUE		:= \033[0;94m
END_COLOR	:= \033[0;39m

# **************************************************************************** #
# VARIABLES

SRCS	=	ft_atoi.c \
			ft_print_nl.c \
			list_utils.c \
			main.c
OBJ		=	${SRCS:.c=.o}
HEADER	=   philosophers.h
NAME	=	philosophers
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -O0

# **************************************************************************** #
# RULES

%.o:	%.c
	@${CC} ${CFLAGS} -c -g $< -o $@
	@echo "$(BLUE)Compiling $<...$(END_COLOR)"

$(NAME):	${OBJ} Makefile
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) ${HEADER} -lpthread
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