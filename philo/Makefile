NAME = philo

OBJECTS = 					\
		main.o				\
		initialization.o	\
		utils.o				\
		check_dead.o		\
		routine.o			\

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJECTS)
	cc -o $@ $^

%.o: %.c
	cc $(CFLAGS) -g -c -o $@ $<

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
