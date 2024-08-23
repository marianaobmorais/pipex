/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/23 20:18:06 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/* typedef struct s_args
{
	char	**args;
	char	*pathname;
	int		fd_in;
	int		fd_out;
}	t_args; */

t_args	*parse_argv(int argc, char **argv, char **envp)
{
	t_args	*args;
	int		i;

	args = (t_args *)malloc(sizeof(t_args) * (argc - 2));
	if (args == NULL)
		return (NULL);
	i = 0;
	while (i < argc - 3)
	{
		args[i].args = ft_split(argv[i + 2], ' ');
		args[i].pathname = get_pathname(args[i].args, envp);
		args[i].fd_in = open(argv[1], O_RDONLY);
		if (args[i].fd_in == -1)
			return (/* free(args), free_vector(args->argv),  */NULL);
		args[i].fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644); // check the permissions
		if (args[i].fd_out == -1)
			return (/* free(args), free_vector(args->argv),  */NULL);
		i++;
	}
	args[i].args = NULL;
	args[i].pathname = NULL;
	return (args);
}

int	main(int argc, char **argv, char **envp)
{
	t_args	*args;
	pid_t	pid;
	int		fd[2];
	int		i;

	if (argc < 5) // bonus
		return (1);
	args = parse_argv(argc, argv, envp);
	if (args == NULL)
		exit(EXIT_FAILURE);
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			return (perror("Pipe creation failed"), 1); // What number do I need to return?
		pid = fork();
		if (pid == -1)
			return (perror("Fork creation failed"), 1);
		if (pid == 0)
			child_process(args, envp, fd, i);
		else
			parent_process(fd, pid);
		i++;
	}
	return (free_vector(args->argv), free(args), 0);
}
