NAME = philo_bonus

FILES = main		\
		init		\
		uninit		\
		utils		\
		routine		\
		eating		\
		check_args	\

OBJECTS = $(FILES:%=obj/%.o)

CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -pthread

all: $(NAME)

$(NAME): $(OBJECTS)
	cc $(LDFLAGS) -o $@ $^

obj:
	mkdir -p $@

obj/%.o: %.c | obj
	cc $(CFLAGS) -g -c -o $@ $<

clean:
	rm -rf $(OBJECTS) obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
