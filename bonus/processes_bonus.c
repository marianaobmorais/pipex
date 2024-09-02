/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/01 17:00:20 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void	exit_child(t_args *args, int *fd, int argc)
{
	close(fd[0]);
	close(fd[1]);
	free_struct(args, argc);
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

static int	open_file(char *file, bool first_child, bool heredoc)
{
	int	fd;

	if (first_child == true)
	{
		if (heredoc == true)
		{
			fd = 10; //
			ft_putstr_fd("fd here_doc\n", 2);
		}
		else
		{
			if (access(file, F_OK) == -1)
				return (ft_printf(ERR_FILE, file), -1);
			fd = open(file, O_RDONLY);
			if (fd == -1)
				return (ft_printf(ERR_PERMISSION, file), fd);
		}
	}
	else
	{
		if (heredoc == true)
			fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (ft_printf(ERR_PERMISSION, file), fd);
	}
	return (fd);
}

/* static int	open_file(char *file, bool infile, bool heredoc)
{
	int	fd;

	fd = -1;
	if (infile == true)
	{
		if (heredoc == true)
			fd = ; //
		else
		{
			if (access(file, F_OK) == -1)
				return (ft_printf(ERR_FILE, file), fd);
			fd = open(file, O_RDONLY);
			if (fd == -1)
				return (ft_printf(ERR_PERMISSION, file), fd);
		}
	}
	else
	{
		if (heredoc == true)
			fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (ft_printf(ERR_PERMISSION, file), fd);
	}
	return (fd);
} */

void	child_process(t_args *args, int c, char **envp, int *fd)
{
	int		fd_file;

	if (args[c].args == NULL || args[c].pathname == NULL)
		exit_child(args, fd, args[c].argc);
	if (args[c].first_child == true)
	{
		fd_file = open_file(args[c].infile, args[c].first_child, args[c].heredoc);
		if (fd_file == -1)
			exit_child(args, fd, args[c].argc);
		dup2(fd_file, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd_file);
	}
	else if (args[c].last_child == true)
	{
		fd_file = open_file(args[c].outfile, args[c].first_child, args[c].heredoc);
		if (fd_file == -1)
			exit_child(args, fd, args[c].argc);
		dup2(fd_file, STDOUT_FILENO);
		close(fd_file);
	}
	else
		dup2(fd[1], STDOUT_FILENO);
	if (execute(args[c].pathname, args[c].args, envp, fd) == -1)
		exit_child(args, fd, args[c].argc);
}

void	parent_process(int *fd, pid_t pid, bool is_heredoc)
{
	if (is_heredoc == true)
		waitpid(pid, NULL, 0);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}
