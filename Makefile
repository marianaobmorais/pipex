# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marianamorais <marianamorais@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/10 17:36:47 by mariaoli          #+#    #+#              #
#    Updated: 2024/08/28 11:58:17 by marianamora      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRCS_DIR = ./srcs

LIBFT = ./libft/libft.a
LIBFT_DIR =./libft

SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/parse_argv.c \
		$(SRCS_DIR)/free_functions.c \
		#$(SRCS_DIR)/processes.c 

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