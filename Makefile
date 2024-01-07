# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ecaruso <ecaruso@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/01 05:55:29 by duzegbu           #+#    #+#              #
#    Updated: 2024/01/07 20:03:35 by ecaruso          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

# LIBFT_A		=	libft.a
LIBFT_EXE	=	libft/libft.a
LIBFT		=	libft

CC			=	gcc
INCLUDE		=	includes
CFLAGS		=	-Wall -Wextra -Werror -I$(INCLUDE)
RM			=	rm -f

EXECUTOR_DIR	=	src/executor
PARSING_DIR	=	src/parser
BUILTIN_DIR	=	src/builtins
SIGNAL_DIR =	src/signals

SRCS = 		minishell.c \
       		$(wildcard $(PARSING_DIR)/*.c) \
       		$(wildcard $(EXECUTOR_DIR)/*.c) \
       	#	$(wildcard $(BUILTIN_DIR)/*.c) \
       		$(wildcard $(SIGNAL_DIR)/*.c) \

OBJS		=	$(SRCS:%.c=%.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			@make -C $(LIBFT)
			@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT_EXE) -lreadline
			@echo "\nLinked into executable \033[0;32mminishell\033[0m."

.c.o:
			@printf "\033[0;33mGenerating minishell objects... %-33.33s\r" $@
			@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

clean:
			@make clean -C $(LIBFT)
			@echo "Clean libft."
			@$(RM) $(OBJS)

fclean:		clean
			@make fclean -C $(LIBFT)
			@echo "Full clean libft."
			@$(RM) $(NAME)
			@echo "Removed executable."

re:			fclean all

.PHONY:		all clean fclean re