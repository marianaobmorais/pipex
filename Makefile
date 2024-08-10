# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/10 17:36:47 by mariaoli          #+#    #+#              #
#    Updated: 2024/08/10 18:08:35 by mariaoli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRCS_DIR = ./srcs

LIBFT = ./libft/libft.a
LIBFT_DIR =./libft

SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/valid_args.c

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

RM = rm -f

$(NAME): $(LIBFT) $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) clean -C $(LIBFT)

fclean:
	$(RM) $(NAME) $(LIBFT)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re