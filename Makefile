NAME=manage_JSON

CC=clang
CFLAGS= -Wall -Werror -Wextra -g3
RM= rm -rf
LIB= -L . -lft
INC= -I ${PWD}/libft/inc

FILES= clasificador \

SRC = $(addsuffix .c, $(FILES))
OBJ = $(addsuffix .o, $(FILES))

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

$(NAME): $(OBJ)
	${MAKE} -C ${PWD}/libft/. 
	mv ${PWD}/libft/libft.a .
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIB)

all: $(NAME)

clean:
	${MAKE} -C ${PWD}/libft/. clean
	$(RM) $(OBJ)

fclean: clean
	${MAKE} -C ${PWD}/libft/. fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all re clean fclean
