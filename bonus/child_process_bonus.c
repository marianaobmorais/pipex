/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/09 15:45:11 by mariaoli         ###   ########.fr       */
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
				return (ft_error(2, ERR_FILE, args[0].infile), -1);
			fd = open(args[0].infile, O_RDONLY);
			if (fd == -1)
				return (ft_error(2, ERR_PERMISSION, args[0].infile), fd);
		}
	}
	else
	{
		if (heredoc == true)
			fd = open(args[0].outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(args[0].outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (ft_error(2, ERR_PERMISSION, args[0].outfile), fd);
	}
	return (fd);
}

static void	is_valid_arg(t_args *args, int c, int *fd)
{
	if (args[c].args[0] == NULL || args[c].args == NULL)
	{
		if (args[c].args == NULL)
			ft_error(2, ERR_PERMISSION, ((char *)0));
		else
			ft_error(2, ERR_PERMISSION, args[c].args[0]);
		exit_child(args, fd);
	}
	else if (access(args[c].pathname, X_OK) == -1)
	{
		if (args[c].pathname == NULL)
			ft_error(2, ERR_COMMAND, ((char *)0));
		else
			ft_error(2, ERR_COMMAND, args[c].pathname);
		exit_child(args, fd);
	}
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

static int	fd_first_child(t_args *args, int c, int *fd)
{
	int	fd_file;

	fd_file = open_file(args, args[c].first_child, args[c].heredoc);
	is_valid_arg(args, c, fd);
	if (fd_file == -1)
		exit_child(args, fd);
	return (fd_file);
}

void	child_process(t_args *args, int c, char **envp, int *fd)
{
	int	fd_file;

	if (args[c].first_child == true)
	{
		fd_file = fd_first_child(args, c, fd);
		dup2(fd_file, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd_file);
	}
	else if (args[c].last_child == true)
	{
		fd_file = open_file(args, args[c].first_child, args[c].heredoc);
		is_valid_arg(args, c, fd);
		if (fd_file == -1)
			exit_child(args, fd);
		dup2(fd_file, STDOUT_FILENO);
		close(fd_file);
	}
	else
	{
		is_valid_arg(args, c, fd);
		dup2(fd[1], STDOUT_FILENO);
	}
	if (execute(args, c, envp, fd) == -1)
		exit_child(args, fd);
}

// 	if (args[c].first_child == true)
// 	{
//	 	fd_file = open_file(args, args[c].first_child, args[c].heredoc);
// 		is_valid_arg(args, c, fd);
// 		if (fd_file == -1)
// 			exit_child(args, fd);
// 		dup2(fd_file, STDIN_FILENO);
// 		dup2(fd[1], STDOUT_FILENO);
// 		close(fd_file);
// 	}