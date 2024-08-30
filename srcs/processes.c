/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/30 12:21:04 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	exit_child(t_args *args, int *fd)
{
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

static int	open_file(char *file, bool infile)
{
	int fd;

	fd = -1;
	if (access(file, F_OK) == -1)
		return (ft_printf(ERR_FILE, file), fd);
	else
	{
		if (infile == true)
			fd = open(file, O_RDONLY);
		else
			fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644); // are the permissions right?
		if (fd == -1)
			return (ft_printf(ERR_PERMISSION, file), fd);
		return (fd);
	}
}

void	child_process(t_args *args, int count, char **envp, int *fd)
{
	int	fd_io;

	if (/* args[count].args == NULL || */ args[count].pathname == NULL)
		exit_child(args, fd);
	if (args[count].first_child == true)
	{
		fd_io = open_file(args[count].infile, true);
		if (fd_io == -1)
			exit_child(args, fd);
		dup2(fd_io, STDIN_FILENO);
		close(fd_io);
	}
	if (args[count].last_child == true)
	{
		fd_io = open_file(args[count].outfile, false);
		if (fd_io == -1)
			exit_child(args, fd);
		dup2(fd_io, STDOUT_FILENO);
		close(fd_io);
	}
	else
		dup2(fd[1], STDOUT_FILENO);
	if (execute(args[count].pathname, args[count].args, envp, fd) == -1)
		exit_child(args, fd);
}

void	parent_process(int *fd)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}
