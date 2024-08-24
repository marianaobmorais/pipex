/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:41:13 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/24 20:07:58 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	execute(t_args *args, char **envp)
{
	int		err;

	err = execve(args->pathname, args->args, envp);
	if (err == -1)
		perror("exevc failed");
	return (err);
}

void	child_process(int argc, t_args *args, char **envp, int *fd, int i)
{
	int	err;
	
	if (i == 2) // first process
	{
		dup2(args->fd_in, STDIN_FILENO); // read - instead of reading from the stdin (keyboard/whatever is written on the terminal), it will read from the av[1]
		close(args->fd_in);
	}
	if (i == argc - 2) // last process
	{
		dup2(args->fd_out, STDOUT_FILENO); // write - everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
		close(args->fd_out);
	}
	else // middle processes
		dup2(fd[1], STDOUT_FILENO); // everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
	close(fd[0]);
	close(fd[1]);
	err = execute(args, envp); // execve should write to the stdout - it inherits from the parent
	if (err == -1)
	{
		free_struct(args);
		exit(EXIT_FAILURE);
	}
}

void	parent_process(int *fd, pid_t pid)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO); // everything that would normally come from stdin (the keyboard) now comes from the read end of the pipe (fd[0]).
	// Redirect stdout to the read end of the pipe
	close(fd[0]);
	waitpid(pid, NULL, 0);
}

/* int	execute(char *arg, char **envp)
{
	int		err;
	char	*pathname;
	char	**args;
	
	err = -1;
	pathname = get_pathname(arg, envp);
	if (pathname == NULL)
		return (err);
	args = get_exec_args(arg);
	if (args == NULL)
		return (err); // malloc failed
	err = execve(pathname, args, envp);
	if (err == -1)
		perror("exevc failed");
	return (err);
}

void	child_process(t_args *args, char **envp, int *fd, int i)
{
	int	err;
	
	if (i == 2) // first process
	{
		dup2(args->fd_in, STDIN_FILENO); // read - instead of reading from the stdin (keyboard/whatever is written on the terminal), it will read from the av[1]
		close(args->fd_in);
	}
	if (i == args->argc - 2) // last process
	{
		dup2(args->fd_out, STDOUT_FILENO); // write - everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
		close(args->fd_out);
	}
	else // middle processes
		dup2(fd[1], STDOUT_FILENO); // everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
	close(fd[0]);
	close(fd[1]);
	err = execute(args->argv[i], envp); // execve should write to the stdout - it inherits from the parent
	if (err == -1)
	{
		free_vector(args->argv);
		free(args);
		exit(EXIT_FAILURE);
	}
}

void	parent_process(int *fd, pid_t pid)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO); // everything that would normally come from stdin (the keyboard) now comes from the read end of the pipe (fd[0]).
	// Redirect stdout to the read end of the pipe
	close(fd[0]);
	waitpid(pid, NULL, 0);
} */
