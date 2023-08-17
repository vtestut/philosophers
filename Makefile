SRCS =	main.c
SRCS += parsing.c 

OBJS = ${SRCS:.c=.o}

#INC = -I inc/ -I libft
#LIBFTPATH = -L libft -lft
LIBS = ${INC} #${LIBFTPATH}

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror 

.c.o:
		${CC} ${CFLAGS} ${INC} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS} 
		+$(MAKE) -C libft
		cc -o ${NAME} ${CFLAGS} ${OBJS} ${LIBS}

all:	${NAME}

bonus:	${NAME}

clean:	
	@	+$(MAKE) -C libft clean
	@	rm -f ${OBJS} ${DEPS} ${OBJSBONUS} ${DEPSBONUS}

fclean:	clean;
	@	+$(MAKE) -C libft fclean
	@	@rm -f ${NAME} ${BONUS}


re:	fclean all