# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: duzegbu <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/01 05:55:29 by duzegbu           #+#    #+#              #
#    Updated: 2023/12/06 16:55:39 by duzegbu          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

LIBFT_A		=	libft.a
LIBFT_DIR	=	libft/
LIBFT		=	$(addprefix $(LIBFT_DIR), $(LIBFT_A))

CC			=	gcc
INCLUDE		=	includes
CFLAGS		=	-Wall -Wextra -Werror -I$(INCLUDE)
RM			=	rm -f

EXECUTOR_DIR	=	srcs/executor
PARSING_DIR	=	srcs/parsing
BUILTIN_DIR	=	srcs/builtins

SRCS		=	minishell.c \
				$(EXEC_DIR)/exec_errors.c \
				$(EXECUTOR_DIR)/executor.c \
				srcs/utils.c \

OBJS		=	$(SRCS:%.c=%.o)

all:		$(NAME)

$(NAME):	$(LIBFT) $(OBJS)
			@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft -L/Users/$(USER)/.brew/opt/readline/lib -lreadline -ltermcap
			@echo "\nLinked into executable \033[0;32mminishell\033[0m."

$(LIBFT):
			@echo "Compiling libft.a"
			@$(MAKE) bonus -s -C $(LIBFT_DIR)

.c.o:
			@printf "\033[0;33mGenerating minishell objects... %-33.33s\r" $@
			@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

localclean:
			@$(RM) $(OBJS)
			@echo "Removed object files."

clean:		localclean
			@$(MAKE) clean -s -C $(LIBFT_DIR)
			@echo "Clean libft."

fclean:		localclean
			@$(MAKE) fclean -s -C $(LIBFT_DIR)
			@echo "Full clean libft."
			@$(RM) $(NAME)
			@echo "Removed executable."

re:			fclean all

.PHONY:		all clean fclean localclean re