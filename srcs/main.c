/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/28 01:43:18 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*get_pathname(char **args, char **envp)
{
	char	*path_slash;
	char	*absolute_pathname;
	char	**paths;
	int		i;

	if (!args[0])
		return (ft_printf("%s: command not found\n", args[0]), NULL);
	if (access(args[0], F_OK) == 0 && access(args[0], X_OK) == 0)
		return (ft_strdup(args[0]));
	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	paths = NULL;
	if (envp[i] != NULL)
		paths = ft_split(envp[i] + 5, ':');
	if (paths == NULL)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		path_slash = ft_strjoin(paths[i], "/");
		//ft_printf("path_slash: %s\n", path_slash); // erase this
		absolute_pathname = ft_strjoin(path_slash, args[0]);
		free(path_slash);
		//ft_printf("absolute path: %s\n", absolute_pathname); // erase this
		if (access(absolute_pathname, F_OK) == 0 && access(absolute_pathname, X_OK) == 0)
			return (free_vector(paths), absolute_pathname);
		free(absolute_pathname);
		i++;
	}
	ft_printf("%s: command not found\n", args[0]);
	return (free_vector(paths), NULL);
}

void	exit_child(char **args, int *fd, t_open fd_files, int i)
{
	if (args == NULL)
	{
		//free(fd_files.infile); // acho que tenho que passar por referencia para poder dar free
		exit (1); // check the proper exit number = memory allocation failed
	}
	if (i == 2)
	{
		//fd_in = open("/dev/null", O_RDONLY); // já aberto na main
		dup2(fd_files.fd_in, STDIN_FILENO); // isso é só pra primeira volta? mas para quê duplicar o fd_in?
		close(fd_files.fd_in);
	}
	close(fd[0]);
	close(fd[1]);
	free_vector(args);
	//free(fd_io.infile); ?
	exit(127);
}

static int	child_process(/* int argc, char *argv, */t_args args, char **envp, int *fd, t_open fd_files/* , int i */)
{
/* 	char	**args;
	char	*pathname; */
	int		err;
	
	//args = ft_split(argv, ' ');
	if (args == NULL)
		exit_child(args, fd, fd_files, i);
	//pathname = get_pathname(args, envp);
	if (pathname == NULL)
		exit_child(args, fd, fd_files, i);
	err = -1;
	if (i == 2) // first process
	{
		if (access(fd_files.infile, F_OK) == -1 || access(fd_files.infile, R_OK) == -1) // if infile doesnt exist or it's not readable
			exit_child(args, fd, fd_files, i);
		else
		{
			dup2(fd_files.fd_in, STDIN_FILENO); // read - instead of reading from the stdin (keyboard/whatever is written on the terminal), it will read from the av[1]
			close(fd_files.fd_in);
		}
	}
	if (i == argc - 2) // last process
	{
		dup2(fd_files.fd_out, STDOUT_FILENO); // write - everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
		close(fd_files.fd_out);
	}
	else // middle processes
		dup2(fd[1], STDOUT_FILENO); // everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
	close(fd[0]);
	close(fd[1]);
	err = execve(pathname, args, envp);
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

	if (access(infile, F_OK) == -1) // checks existence of infile
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
	fd_files.infile = ft_strdup(infile); // do I need to check if fd_files.infile == NULL?
	//fd_files.outfile = ft_strdup(outfile);
	return (fd_files);
}

/* typedef struct s_args
{
	char	**args;
	char	*pathname;
	bool	first_child;
	bool	last_child;
}	t_args; */

t_args	init_args(int argc, char **argv, char **envp)
{
	
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	t_open	fd_files;
	t_args	args;
	int		fd[2];
	int		i;

	if (argc < 5) // change this
		return (1);
	fd_files = open_files(argv[1], argv[argc - 1]);
	args = init_args(argc, argv, envp);
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
			child_process(/* argc, argv[i], */args, envp, fd, fd_files/* , i */); // passar o fd_files por referencia para poder dar free mais na frente?
		else
			parent_process(fd, pid);
		i++;
	}
	free(fd_files.infile);
	return (0);
}
