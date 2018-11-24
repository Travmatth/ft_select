DEBUG =
ifdef TEST
        DEBUG = -g -fsanitize=address -fsanitize=undefined
else
        DEBUG =
endif
ifdef LEAKS
        DEBUG = -g
endif

NAME = ft_select
LIBFT = libftprintf/libftprintf.a
CFLAGS += -Wall -Wextra -Werror -Wpedantic
LDFLAGS := -Llibftprintf -lftprintf -I./includes -ltermcap
CORE := main parse write_lines events signal utils
FILES := $(addprefix src/, $(CORE))
SRC := $(addsuffix .c, $(FILES))
OBJ := $(SRC:.c=.o)

.PHONY = all clean fclean re

all: $(SUBMODULES) $(NAME)

$(SUBMODULES):
	@git submodule init
	@git submodule update

$(LIBFT):
	@$(MAKE) -C libftprintf

$(OBJ): %.o: %.c
	@$(CC) -c $(DEBUG) -I. $(CFLAGS) $< -o $@

$(NAME): $(LIBFT) $(OBJ)
	@echo -n 'Compiling ft_select... '
	@$(CC) $(DEBUG) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@
	@echo "\033[32mdone\033[0m"

clean:
	@$(MAKE) clean -C libftprintf
	@echo -n 'Cleaning ft_select object files... '
	@rm -f $(OBJ) *.dSYM *.DS_Store
	@echo "\033[32mdone\033[0m"

fclean: clean
	@$(MAKE) fclean -C libftprintf
	@echo -n 'Cleaning ft_select executable... '
	@rm -f $(NAME)
	@echo "\033[32mdone\033[0m"

re: fclean all
