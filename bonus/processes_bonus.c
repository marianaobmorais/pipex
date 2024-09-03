/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/03 19:50:20 by mariaoli         ###   ########.fr       */
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

static void	exit_child(t_args *args, int *fd, int c)
{
	int	fd_io;

	if (access(args[c].outfile, W_OK) != -1)
		fd_io = open_file(args, args[c].first_child, args[c].heredoc);
	else if (access(args[c].outfile, F_OK) == -1)
		fd_io = open_file(args, args[c].first_child, args[c].heredoc);
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

void	child_process(t_args *args, int c, char **envp, int *fd)
{
	int		fd_file;

	if (args[c].args == NULL || args[c].pathname == NULL)
		exit_child(args, fd, c);
	if (args[c].first_child == true)
	{
		fd_file = open_file(args, args[c].first_child, args[c].heredoc);
		if (fd_file == -1)
			exit_child(args, fd, c);
		dup2(fd_file, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd_file);
	}
	else if (args[c].last_child == true)
	{
		fd_file = open_file(args, args[c].first_child, args[c].heredoc);
		if (fd_file == -1)
			exit_child(args, fd, c);
		dup2(fd_file, STDOUT_FILENO);
		close(fd_file);
	}
	else
		dup2(fd[1], STDOUT_FILENO);
	if (execute(args[c].pathname, args[c].args, envp, fd) == -1)
		exit_child(args, fd, c);
}

void	parent_process(int *fd, pid_t pid, bool is_heredoc)
{
	if (is_heredoc == true)
		waitpid(pid, NULL, 0);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}
