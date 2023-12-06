# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: grinella <grinella@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/25 11:30:51 by grinella          #+#    #+#              #
#    Updated: 2023/02/02 19:20:56 by grinella         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = ft_atoi.c \
	  	ft_calloc.c \
		ft_isalpha.c \
		ft_isprint.c \
		ft_memcpy.c \
		ft_strchr.c \
		ft_strlen.c \
		ft_strrchr.c \
		ft_bzero.c \
		ft_isascii.c \
		ft_memchr.c \
		ft_memmove.c \
		ft_strlcat.c \
		ft_strncmp.c \
		ft_tolower.c \
		ft_isalnum.c \
		ft_isdigit.c \
		ft_memcmp.c \
		ft_memset.c \
		ft_strlcpy.c \
		ft_strnstr.c \
		ft_toupper.c \
		ft_strdup.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_split.c \
		ft_itoa.c \
		ft_strmapi.c \
		ft_putchar_fd.c \
		ft_striteri.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \

SRCB = ft_lstnew.c \
		ft_lstadd_front.c \
		ft_lstsize.c \
		ft_lstlast.c \
		ft_lstadd_back.c \
		ft_lstdelone.c \
		ft_lstclear.c \
		ft_lstiter.c \
		ft_lstmap.c \

FLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)
OBJB = $(SRCB:.c=.o)
all: $(NAME)

bonus: $(OBJB)
	ar rc $(NAME) $(OBJB)
	ranlib $(NAME)
	
$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

%.o: %.c
	gcc $(FLAGS) -c $< -o $@
clean:
	rm -f $(OBJ) $(OBJB)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re
