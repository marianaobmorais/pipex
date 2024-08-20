/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/20 17:58:45 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	first_child(int *fd, char **av, char **envp)
{
	int	fd_in;
	int	err;
	
	close(fd[0]); // close the read end
	fd_in = open(av[1], O_RDONLY);
	if (fd_in == -1)
		return (perror("Error opening input file"), 2);
	dup2(fd_in, STDIN_FILENO); // read
	dup2(fd[1], STDOUT_FILENO); // everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
	close(fd_in);
	close(fd[1]);
	char *pathname = get_pathname(av[2], envp);
	char **args = get_exec_args(av[2]);
	err = execve(pathname, args, NULL); // If you pass NULL for the envp parameter in execve, it inherits the environment of the calling (parent) process. This way, you don’t lose the PATH variable.
	if (err == -1)
	{
		perror("execve failed");
		exit (EXIT_FAILURE);
		//return (perror("execve failed"), 2);
	}
	return (err);
}

int	last_child(int *fd, int ac, char **av, char **envp)
{
	int	fd_out;
	int	err;
	
	close(fd[1]); // close the write end
	fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd[0], STDIN_FILENO); // everything that would normally come from stdin (the keyboard) now comes from the read end of the pipe (fd[0]).
	dup2(fd_out, STDOUT_FILENO); // write
	close (fd_out);
	close(fd[0]);
	char *pathname = get_pathname(av[ac - 2], envp);
	char **args = get_exec_args(av[ac - 2]);
	err = execve(pathname, args, NULL);
	if (err == -1)
	{
		perror("execve failed");
		exit (EXIT_FAILURE);
		//return (perror("execve failed"), 2);
	}
	return (err);
}
