NAME = philo

FILES = main			\
		initialization	\
		utils			\
		routine			\
		eating			\
		check_args		\
		sister			\

OBJECTS = $(FILES:%=obj/%.o)

CFLAGS = -Wall -Wextra -Werror -fsanitize=thread -O1
LDFLAGS = -pthread -fsanitize=thread -O1

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
