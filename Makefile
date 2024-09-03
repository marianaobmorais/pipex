# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/10 17:36:47 by mariaoli          #+#    #+#              #
#    Updated: 2024/09/03 18:09:07 by mariaoli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
BONUS_NAME = pipex_bonus

SRCS_DIR = ./srcs
BONUS_DIR = ./bonus

LIBFT = ./libft/libft.a
LIBFT_DIR =./libft

SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/parse_argv.c \
		$(SRCS_DIR)/free_functions.c \
		$(SRCS_DIR)/processes.c

BONUS_SRCS = $(BONUS_DIR)/main_bonus.c \
		$(BONUS_DIR)/parse_argv_bonus.c \
		$(BONUS_DIR)/free_functions_bonus.c \
		$(BONUS_DIR)/processes_bonus.c \
		$(BONUS_DIR)/heredoc_utils.c

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

RM = rm -f

$(NAME): $(LIBFT) $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(LIBFT) $(BONUS_SRCS)
	$(CC) $(CFLAGS) $(BONUS_SRCS) $(LIBFT) -o $(BONUS_NAME)

all: $(NAME) bonus

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) clean -C $(LIBFT)

fclean:
	$(RM) $(NAME) $(BONUS_NAME) $(LIBFT)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re bonus