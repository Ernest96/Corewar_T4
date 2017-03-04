NAME = asm

CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC = *.c ../libft/*.c

OBJ = *.o

all: $(NAME)

$(NAME):
	@$(CC) $(CFLAGS) $(SRC) -c 
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm  $(OBJ)

fclean: clean
	rm $(NAME)

re: fclean all
