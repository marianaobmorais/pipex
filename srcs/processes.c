/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/28 20:26:02 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	exit_child(t_args *args, int count, t_open fd)
{
	if (args[count].args == NULL)
	{
		free(fd.infile);
		exit(1); //number??
	}
/* 	if (args[count].first_child == true)
	{
		//fd_in = open("/dev/null", O_RDONLY); // já aberto na main
		dup2(fd.fd_in, STDIN_FILENO); // isso é só pra primeira volta? mas para quê duplicar o fd_in?
		close(fd.fd_in);
	} */
	close(fd.fd[0]);
	close(fd.fd[1]);
	free_struct(args);
	free(fd.infile);
	exit(126);
}

int	execute(char *pathname, char **args, char **envp, t_open fd)
{
	int	err;

	err = -1;
	close(fd.fd[0]);
	close(fd.fd[1]);
	err = execve(pathname, args, envp);
	if (err == -1)
		perror("execve");
	return (err);
}

void	child_process(t_args *args, int count, char **envp, t_open fd)
{
	if (args[count].args == NULL || args[count].pathname == NULL)
		exit_child(args, count, fd);
	if (args[count].first_child == true) // first process
	{
		if (access(fd.infile, F_OK) == -1 || access(fd.infile, R_OK) == -1) // if infile doesnt exist or it's not readable
			exit_child(args, count, fd);
		else
		{
			dup2(fd.fd_in, STDIN_FILENO); // read - instead of reading from the stdin (keyboard/whatever is written on the terminal), it will read from the av[1]
			close(fd.fd_in);
		}
	}
	if (args[count].last_child == true) // last process
	{
		dup2(fd.fd_out, STDOUT_FILENO); // write - everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
		close(fd.fd_out);
	}
	else // middle processes
		dup2(fd.fd[1], STDOUT_FILENO); // everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
	if (execute(args[count].pathname, args[count].args, envp, fd) == -1)
		exit_child(args, count, fd);
}

void	parent_process(int *fd)
{
	dup2(fd[0], STDIN_FILENO); // everything that would normally come from stdin (the keyboard) now comes from the read end of the pipe (fd[0]).
	// Redirect stdout to the read end of the pipe
	close(fd[0]);
	close(fd[1]);
}
