NAME = philo

OBJECTS = 					\
		main.o				\
		initialization.o	\
		utils.o				\
		routine.o			\
		watcher.o			\
		eating.o			\
		check_args.o		\

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
