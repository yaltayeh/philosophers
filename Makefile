NAME = philo

OBJECTS = main.o

all: $(NAME)

$(NAME): $(OBJECTS)
	cc -o $@ $^

%.o: %.c
	cc -Wall -Wextra -Werror -c -o $@ $<

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
