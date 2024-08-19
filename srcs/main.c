/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/19 21:19:50 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/* This function extracts the first word from the string s up to 
the first occurrence of the character c. It allocates memory for 
the first word and returns it. */

char	*get_first_word(char *s, int c)
{
	char	*first_word;
	int		i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
		i++;
	first_word = (char *)malloc(sizeof(char) * (i + 1));
	if (first_word == NULL)
		return (NULL);
	i = 0;
	while (s[i] != c && s[i] != '\0')
	{
		first_word[i] = s[i];
		i++;
	}
	first_word[i] = '\0';
	return (first_word);
}

/* This function aims to retrieve the executable path based 
on the argv input and the environment variables (envp). 
It uses get_first_word to extract the command name from argv. 
Then it checks whether the command is an absolute path (starting with /). 
If it is, it checks for the command's existence and returns it if found. 
Otherwise, it searches for the command in the directories specified by the 
PATH environment variable. */

char	*get_pathname(char *argv, char **envp)
{
	(void)envp;
	char	*pathname;
	char	*slash_pathname;
	char	*new_pathname;
	char	**paths;
	
	pathname = get_first_word(argv, ' ');
	if (ft_strncmp(pathname, "/", 1) == 0)
	{
		if (access(pathname, F_OK) == -1)
		{
			perror(pathname);
			free(pathname);
			exit(1);
		}
		return (pathname);
	}
	else
	{
		int i = 0;
		while (envp[i] != NULL)
		{
			if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			{
				paths = ft_split(envp[i] + 5, ':');
				slash_pathname = ft_strjoin("/", pathname);
				int j = 0;
				while (paths[j] != NULL)
				{
					new_pathname = ft_strjoin(paths[j], slash_pathname);
					if (access(new_pathname, F_OK) == 0)
					{
						free_vector(paths);
						free(slash_pathname);
						free(pathname);
						return (new_pathname);
					}
					free(new_pathname);
					j++;
				}
			}
			i++;
		}
		if (access(pathname, F_OK) == -1)
		{
			perror(pathname); // como fazer para ele printar certo? tipo < infile /usr/bn/ls -l | wc -l > outfile = zsh: no such file or directory: /usr/bn/ls
			free_vector(paths);
			free(slash_pathname);
			free(pathname);
			exit(1);
		}
	}
	return (pathname);
}

int	main(int ac, char **av , char **envp)
{
	int	fd[2];
	int	fd_in; // this can go to the child1 function
	int	fd_out; // this can go to the child2 function
	int	pid1;
	int	pid2;
	int	err;

	if (ac != 5)
		return (1);
	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == -1)
		return (perror("Error during fork"), 1); // What number do I need to return?
	if (pid1 == 0)
	{
		close(fd[0]); // close the read end
		fd_in = open(av[1], O_RDONLY);
		if (fd_in == -1)
			return (perror("Error opening input file"), 2);
		dup2(fd_in, STDIN_FILENO); // read
		dup2(fd[1], STDOUT_FILENO); // everything that would normally go to stdout (the terminal) now goes into the write end of the pipe (fd[1]).
		close(fd_in);
		close(fd[1]);
		// char *pathname = "/usr/bin/ls"; // "/bin/ls" also works
		// int path_valid = access(pathname, F_OK);
		// if (path_valid == -1)
		// 	return (perror(pathname), 2); // como fazer para ele printar certo? tipo < infile /usr/bn/ls -l | wc -l > outfile = zsh: no such file or directory: /usr/bn/ls
		char *args[] = {"ls", "-l", NULL};
		char *pathname = get_pathname(av[2], envp);
		err = execve(pathname, args, NULL); // If you pass NULL for the envp parameter in execve, it inherits the environment of the calling (parent) process. This way, you don’t lose the PATH variable.
		if (err == -1)
			return (perror("execve failed"), 2);
	}
	pid2 = fork();
	if (pid2 == -1)
		return (1);
	if (pid2 == 0)
	{
		close(fd[1]); // close the write end
		fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd[0], STDIN_FILENO); // everything that would normally come from stdin (the keyboard) now comes from the read end of the pipe (fd[0]).
		dup2(fd_out, STDOUT_FILENO); // write
		close (fd_out);
		close(fd[0]);
		err = execlp("wc", "wc", "-l", NULL);
		if (err == -1)
		{
			printf("Could not find program to execute or other error ocurred\n");
			return 2;
		}
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}


//The child proccess goes first and should end first

/* int	main(int ac, char **av)
{
	if (!valid_args(ac, av))
		return (EXIT_FAILURE);

	int	fd[2];
	// two different fd for two different ends of a pipe
	// fd[0] - read end
	// fd[1] - write end
	int	pid;
	int	x;
	int	y;

	if (pipe(fd) == -1)
		return (ft_printf("Error occured while opening the pipe\n"), 1);
	pid = fork(); //the fd declared in the beginning are copied over - they are inherited by processes created in fork
	if (pid == -1)
		return (ft_printf("Error occured with the fork\n"), 4);
	if (pid == 0) // This is for the child process. The child process is the first to run
	{
		close(fd[0]);
		ft_printf("Please, input a number: ");
		scanf("%d", &x);
		if(write(fd[1], &x, sizeof(int)) == -1)
			return (ft_printf("Error occured while writing into the pipe\n"), 2);
		close(fd[1]);
	}
	else // This is for the parent process. I runs after all the child processes
	{
		close(fd[1]);
		if (read(fd[0], &y, sizeof(int)) == -1)
		{
			ft_printf("Error occured while reading from the pipe\n");
			return (3);
		}
		//y = y * 3;
		close(fd[0]);
	}
	
	return (0);
} */
