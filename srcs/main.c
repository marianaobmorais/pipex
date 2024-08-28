/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/28 12:13:09 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	exit_child(t_args args, int *fd, t_open fd_files)
{
	if (args.args == NULL)
	{
		free(fd_files.infile); // do I need to pass a pointer to free this?
		exit (1); // check the proper exit number for memory allocation failed
	}
	if (args.first_child == true)
	{
		//fd_in = open("/dev/null", O_RDONLY); // já aberto na main
		dup2(fd_files.fd_in, STDIN_FILENO); // isso é só pra primeira volta? mas para quê duplicar o fd_in?
		close(fd_files.fd_in);
	}
	close(fd[0]);
	close(fd[1]);
	free_vector(args.args);
	free(fd_files.infile);
	exit(127);
}

static int	child_process(t_args args, char **envp, int *fd, t_open fd_files)
{
	int		err;
	
	if (args.args == NULL || args.pathname == NULL)
		exit_child(args, fd, fd_files);
	err = -1;
	if (args.first_child == true) // first process
	{
		if (access(fd_files.infile, F_OK) == -1 || access(fd_files.infile, R_OK) == -1) // if infile doesnt exist or it's not readable
			exit_child(args, fd, fd_files);
		else
		{
			dup2(fd_files.fd_in, STDIN_FILENO); // read - instead of reading from the stdin (keyboard/whatever is written on the terminal), it will read from the av[1]
			close(fd_files.fd_in);
		}
	}
	if (args.last_child == true) // last process
	{
		dup2(fd_files.fd_out, STDOUT_FILENO); // write - everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
		close(fd_files.fd_out);
	}
	else // middle processes
		dup2(fd[1], STDOUT_FILENO); // everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
	close(fd[0]);
	close(fd[1]);
	err = execve(args.pathname, args.args, envp);
	if (err == -1)
		perror("execve");
	return (err);
}

void	parent_process(int *fd, pid_t pid)
{
	dup2(fd[0], STDIN_FILENO); // everything that would normally come from stdin (the keyboard) now comes from the read end of the pipe (fd[0]).
	// Redirect stdout to the read end of the pipe
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, NULL, 0);
}

t_open	open_files(char *infile, char *outfile)
{
	t_open fd_files;

	fd_files.infile = ft_strdup(infile); // do I need to check if fd_files.infile == NULL?
	if (access(fd_files.infile, F_OK) == -1) // checks existence of infile
	{
		ft_printf("%s: no such file or directory\n", infile);
		fd_files.fd_in = open("/dev/null", O_RDONLY); // fd_in is not -1
	}
	else
	{
		fd_files.fd_in = open(infile, O_RDONLY);
		if (fd_files.fd_in == -1) // checks readability of infile
		{
			ft_printf("Error: permission denied: %s\n", infile); // this is for when "chmod 000 infile"
			fd_files.fd_in = open("/dev/null", O_RDONLY); // fd_in is no longer -1
		}
	}
	fd_files.fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); // are the permissions right?
	if (fd_files.fd_out == -1)
		ft_printf("Error: permission denied: %s\n", outfile);
	return (fd_files);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	t_open	fd_files;
	t_args	*args;
	int		fd[2];
	int		i;

	if (argc < 5) // change this
		return (1);
	args = parse_argv(argc, argv, envp);
	fd_files = open_files(argv[1], argv[argc - 1]);
	//ft_printf("fd_in = %d, fd_out = %d\n", fd_io.fd_in, fd_io.fd_out); // why are fd_in and fd_out the same value?
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			return (perror("Pipe creation failed"), 1); // What error number do I need to return?
		pid = fork();
		if (pid == -1)
			return (perror("Fork creation failed"), 1);
		if (pid == 0)
			child_process(args[i - 2], envp, fd, fd_files); // passar o fd_files por referencia para poder dar free mais na frente?
		else
			parent_process(fd, pid);
		i++;
	}
	return (free(fd_files.infile), free_struct(args), 0);
}
