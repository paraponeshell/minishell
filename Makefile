NAME= minishell
CC= cc
CFLAGS = -Wall -Wextra -Werror -I$(LIBFTDIR) -ggdb
SRCS=		main.c\
		src/Parsing/parser.c\
		src/Parsing/parser_utilities.c\
		src/Parsing/parser_utilities_2.c\
		src/Parsing/parser_utilities_3.c\
		src/Parsing/env.c\
		src/Parsing/first_split.c\
		src/Parsing/first_split_utilities.c\
		src/Parsing/listing.c\
		src/Parsing/listing_utilities.c\
		src/Parsing/listing_utilities_2.c\
		src/Parsing/listing_utilities_3.c\
		src/Parsing/second_split.c\
		src/Parsing/valid_line.c\
		src/Parsing/utilities_bordel.c\
		src/Pipes/pipes.c\
		src/Pipes/pipes_utils.c\
		src/Pipes/replacing.c\
		src/Pipes/replacing_utils.c\
		src/Pipes/replacing_utils_2.c\
		src/Pipes/replacing_utils_3.c\
		src/Pipes/execute.c\
		src/Pipes/execute_utils.c\
		src/Pipes/execute_apply_redirection.c\
		src/Pipes/more_execute.c\
		src/Pipes/input.c\
		src/Pipes/output.c\
		src/Pipes/output_utils.c\
		src/Pipes/utils.c\
		src/Pipes/exit_status.c\
		src/Pipes/wildcard.c\
		src/Pipes/wildcard_utils.c\
		src/Pipes/relative_execute.c\
		src/builtin/cd.c\
		src/builtin/cd_utils.c\
		src/builtin/echo.c\
		src/builtin/env.c\
		src/builtin/pwd.c\
		src/builtin/export.c\
		src/builtin/export_utils.c\
		src/builtin/env_utils.c\
		src/builtin/unset.c\
		src/builtin/exit.c\
		src/builtin/echo_utils.c\
		src/builtin/echo_utils_2.c\
		src/builtin/export_alphanum_utils.c\
		src/signal/signals.c\
		gnl/get_next_line.c\
		gnl/get_next_line_utils.c
OBJS= $(SRCS:.c=.o)
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline $(LIBFT)
$(LIBFT):
	@make -C $(LIBFTDIR) > /dev/null
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS)
	@make clean -C $(LIBFTDIR) > /dev/null
fclean: clean
	rm -f $(NAME)
	@make fclean -C $(LIBFTDIR) > /dev/null
all: $(NAME)
re: fclean all
.PHONY: clean fclean all re
