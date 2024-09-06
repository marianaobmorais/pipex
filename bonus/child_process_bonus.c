/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/06 17:34:27 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static int	open_file(t_args *args, bool first_child, bool heredoc)
{
	int	fd;

	if (first_child == true)
	{
		if (heredoc == true)
			fd = heredoc_fd(args[0].limiter);
		else
		{
			if (access(args[0].infile, F_OK) == -1)
				return (ft_printf(ERR_FILE, args[0].infile), -1);
			fd = open(args[0].infile, O_RDONLY);
			if (fd == -1)
				return (ft_printf(ERR_PERMISSION, args[0].infile), fd);
		}
	}
	else
	{
		if (heredoc == true)
			fd = open(args[0].outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(args[0].outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (ft_printf(ERR_PERMISSION, args[0].outfile), fd);
	}
	return (fd);
}

static void	exit_child(t_args *args, int *fd)
{
	close(fd[0]);
	close(fd[1]);
	free_struct(args);
	exit(126);
}

static int	execute(t_args *args, int c, char **envp, int *fd)
{
	int	err;

	err = -1;
	close(fd[0]);
	close(fd[1]);
	err = execve(args[c].pathname, args[c].args, envp);
	if (err == -1)
		perror(PERR_EXECVE);
	return (err);
}

static void	is_valid_arg(t_args *args, int c, int *fd, int fd_file)
{
	if (args[c].args[0] == NULL || args[c].args == NULL)
	{
		close(fd_file);
		ft_printf(ERR_PERMISSION, args[c].args);
		exit_child(args, fd);
	}
	else if (access(args[c].pathname, X_OK) == -1)
	{
		close(fd_file);
		ft_printf(ERR_COMMAND, args[0]);
		exit_child(args, fd);
	}
}

void	child_process(t_args *args, int c, char **envp, int *fd)
{
	int	fd_file;

	if (args[c].first_child == true)
	{
		fd_file = open_file(args, args[c].first_child, args[c].heredoc);
		is_valid_arg(args, c, fd, fd_file);
		if (fd_file == -1)
			exit_child(args, fd);
		dup2(fd_file, STDIN_FILENO);
		close(fd_file);
	}
	if (args[c].last_child == true)
	{
		fd_file = open_file(args, args[c].first_child, args[c].heredoc);
		is_valid_arg(args, c, fd, fd_file);
		if (fd_file == -1)
			exit_child(args, fd);
		dup2(fd_file, STDOUT_FILENO);
		close(fd_file);
	}
	else
		dup2(fd[1], STDOUT_FILENO);
	if (execute(args, c, envp, fd) == -1)
		exit_child(args, fd);
}
