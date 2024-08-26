/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/26 21:07:48 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*get_pathname(char **args, char **envp)
{
	char	*path_slash;
	char	*absolute_pathname;
	char	**paths;
	int		i;

	//ft_printf("args[0]: %s\n", args[0]); // erase
	if (!args[0])
		return (ft_printf("permission denied: %s]\n", args[0]),NULL); // permission denied: 
	if (access(args[0], F_OK) == 0 && access(args[0], X_OK) == 0)
		return (ft_strdup(args[0])); // do I need to allocate mem?
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
	ft_printf("command not found: %s\n", args[0]); // erase this later
	return (free_vector(paths), NULL);
}

static int	child_process(int argc, char *argv, char **envp, int *fd, int fd_in, int fd_out, int i)
{
	int	err;
	
	err = -1;
	char **args = ft_split(argv, ' ');
	char *pathname = get_pathname(args, envp);
	if (pathname == NULL)
	{
		fd_in = open("/dev/null", O_RDONLY);
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		close(fd[0]);
		close(fd[1]);
		free_vector(args);
		ft_putstr_fd("return\n", STDERR_FILENO);
		exit(127);
	}
	else if (i == 2) // first process
	{
		dup2(fd_in, STDIN_FILENO); // read - instead of reading from the stdin (keyboard/whatever is written on the terminal), it will read from the av[1]
		close(fd_in);
	}
	if (i == argc - 2) // last process
	{
		dup2(fd_out, STDOUT_FILENO); // write - everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
		close(fd_out);
	}
	else // middle processes
		dup2(fd[1], STDOUT_FILENO); // everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
	ft_putstr_fd("about to close\n", STDERR_FILENO);
	close(fd[0]);
	close(fd[1]);
	ft_putstr_fd("execve\n", STDERR_FILENO);
	err = execve(pathname, args, envp);
	if (err == -1)
		perror("execve");
	return (err);
}

void	parent_process(int *fd, pid_t pid)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO); // everything that would normally come from stdin (the keyboard) now comes from the read end of the pipe (fd[0]).
	// Redirect stdout to the read end of the pipe
	close(fd[0]);
	waitpid(pid, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		fd[2];
	int		i;

	if (argc < 5) // change this
		return (1);
	int fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644); // are the permissions right?
	if (fd_out == -1)
		ft_printf("Error: permission denied: %s\n", argv[argc - 1]);
	int fd_in = open(argv[1], O_RDONLY);
	if (fd_in == -1)
		ft_printf("Error: permission denied: %s\n", argv[1]);
	//dup2(fd_in, STDIN_FILENO);
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			return (perror("Pipe creation failed"), 1); // What error number do I need to return?
		pid = fork();
		if (pid == -1)
			return (perror("Fork creation failed"), 1);
		if (pid == 0)
			child_process(argc, argv[i], envp, fd, fd_in, fd_out, i);
		else
			parent_process(fd, pid);
		i++;
	}
	return (0);
}
