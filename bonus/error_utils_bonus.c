/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:10:37 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/09 15:45:23 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	ft_putstr_fd_len(char *s, int fd, int len)
{
	if (s == NULL)
		return (ft_putstr_fd_len("(null)", fd, 6));
	write(fd, s, len);
}

void	ft_error(int fd, char *error_msg, char *filename)
{
	if (error_msg != NULL)
		ft_putstr_fd_len(error_msg, 2, ft_strlen(error_msg));
	if (filename != NULL)
		ft_putstr_fd_len(filename, 2, ft_strlen(filename));
	write(fd, "\n", 1);
}

void	exit_child(t_args *args, int *fd)
{
	close(fd[0]);
	close(fd[1]);
	free_struct(args);
	exit(126);
}
