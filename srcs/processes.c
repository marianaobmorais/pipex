/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/03 19:35:08 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static int	open_file(char *file, bool first_child)
{
	int	fd;

	fd = -1;
	if (first_child == true)
	{
		if (access(file, F_OK) == -1)
			return (ft_printf(ERR_FILE, file), fd);
		fd = open(file, O_RDONLY);
		if (fd == -1)
			return (ft_printf(ERR_PERMISSION, file), fd);
	}
	else
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (ft_printf(ERR_PERMISSION, file), fd);
	}
	return (fd);
}

static void	exit_child(t_args *args, int *fd, int count)
{
	int	fd_io;

	if (access(args[count].outfile, W_OK) != -1)
		fd_io = open_file(args[count].outfile, false);
	else if (access(args[count].outfile, F_OK) == -1)
		fd_io = open_file(args[count].outfile, false);
	close(fd[0]);
	close(fd[1]);
	free_struct(args);
	exit(126);
}

static int	execute(char *pathname, char **args, char **envp, int *fd)
{
	int	err;

	err = -1;
	close(fd[0]);
	close(fd[1]);
	err = execve(pathname, args, envp);
	if (err == -1)
		perror(PERR_EXECVE);
	return (err);
}

void	child_process(t_args *args, int count, char **envp, int *fd)
{
	int	fd_io;

	if (args[count].args == NULL || args[count].pathname == NULL)
		exit_child(args, fd, count);
	if (args[count].first_child == true)
	{
		fd_io = open_file(args[count].infile, true);
		if (fd_io == -1)
			exit_child(args, fd, count);
		dup2(fd_io, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd_io);
	}
	else if (args[count].last_child == true)
	{
		fd_io = open_file(args[count].outfile, false);
		if (fd_io == -1)
			exit_child(args, fd, count);
		dup2(fd_io, STDOUT_FILENO);
		close(fd_io);
	}
	if (execute(args[count].pathname, args[count].args, envp, fd) == -1)
		exit_child(args, fd, count);
}

void	parent_process(int *fd)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}
