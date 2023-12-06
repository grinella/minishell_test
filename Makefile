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

NAME = minishell

SRC_DIR = src
PARSER_DIR = parser
EXECUTOR_DIR = executor
BUILTINS_DIR = builtins
SIGNALS_DIR = signals
UTILS_DIR = utils

SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/$(PARSER_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/$(EXECUTOR_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/$(BUILTINS_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/$(SIGNALS_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/$(UTILS_DIR)/*.c)

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iincludes -Ilibft

LIBFT = libft.a
LIBFT_PATH = libft

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT_PATH)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -L$(LIBFT_PATH) -lft -lreadline

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make clean -C $(LIBFT_PATH)
	@rm -f $(OBJS)

fclean: clean
	@make fclean -C $(LIBFT_PATH)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re