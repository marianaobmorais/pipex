/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/04 17:36:37 by mariaoli         ###   ########.fr       */
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
	/* int	fd_io; */

/* 	if (access(args[count].outfile, W_OK) != -1)
		fd_io = open_file(args[count].outfile, false);
	else if (access(args[count].outfile, F_OK) == -1)
		fd_io = open_file(args[count].outfile, false); */
	count = -1; // placeholder
	close(fd[0]);
	close(fd[1]);
	free_struct(args);
	exit(126);
}

static int	execute(t_args *args, int count, char **envp, int *fd)
{
	int	err;

	err = -1;
	close(fd[0]);
	close(fd[1]);
	err = execve(args[count].pathname, args[count].args, envp);
	if (err == -1)
		perror(PERR_EXECVE);
	return (err);
}

void	is_valid_arg(t_args *args, int count, int *fd)
{
	if (args[count].args[0] == NULL || args[count].args == NULL)
	{
		ft_printf(ERR_PERMISSION, args[count].args);
		exit_child(args, fd, count);
	}
	else if (access(args[count].pathname, X_OK) == -1)
	{
		ft_printf(ERR_COMMAND, args[0]);
		exit_child(args, fd, count);
	}
}

void	child_process(t_args *args, int count, char **envp, int *fd)
{
	int	fd_file;

	if (args[count].first_child == true)
	{
		fd_file = open_file(args[count].infile, true);
		is_valid_arg(args, count, fd);
		if (fd_file == -1)
			exit_child(args, fd, count);
		dup2(fd_file, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd_file);
	}
	else if (args[count].last_child == true)
	{
		fd_file = open_file(args[count].outfile, false);
		is_valid_arg(args, count, fd);
		if (fd_file == -1)
			exit_child(args, fd, count);
		dup2(fd_file, STDOUT_FILENO);
		close(fd_file);
	}
	if (execute(args, count, envp, fd) == -1)
		exit_child(args, fd, count);
}

void	parent_process(int *fd)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}
