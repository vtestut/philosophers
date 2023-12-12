NAME	= philo

SRC		=	01_main.c
SRC		+=	02_parser.c
SRC		+=	03_init.c
SRC		+=	04_philo.c
SRC		+=	05_checker.c
SRC		+=	06_utils.c 

CC		= gcc
CFLAGS	= -Werror -Wall -Wextra -pthread #-g #fsanitize=thread -g

SRC_PATH = sources/
OBJ_PATH = objs/
			
SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./include/

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean