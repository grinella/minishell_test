# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ecaruso <ecaruso@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/01 05:55:29 by duzegbu           #+#    #+#              #
#    Updated: 2023/12/06 19:38:43 by ecaruso          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	= minishell

SIGNALS_DIR = src/signals
EXEC_DIR = src/executor
PARSER_DIR = src/parser


SRCS 	= 	minishell.c \
			$(wildcard $(SIGNALS_DIR)/*.c) \
			$(wildcard $(EXEC_DIR)/*.c) \
			$(wildcard $(PARSER_DIR)/*.c) \

#libft

LIBFT_PATH 	= ./libft

LIBFT 	= $(LIBFT_PATH)


CC 		= gcc

CFLAGS 	= -Wall -Wextra -Werror -lreadline

OBJS 	= $(SRCS:.c=.o)

%.o: %.c
			$(CC) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
		@echo "$(COLOUR_YELLOW)COMPILING...$(COLOUR_END)"
		make -C $(LIBFT_PATH)
		$(CC) $(OBJS) $(LIBFT)/libft.a -o $(NAME) $(CFLAGS)
		@echo "$(COLOUR_GREEN)READY TO GO!$(COLOUR_END)"

clean:
		rm -f $(OBJS)
		make fclean -C ${LIBFT_PATH}

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re