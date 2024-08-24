/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/24 19:59:48 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_args	*args;
	pid_t	pid;
	int		fd[2];
	int		i;

	if (argc < 5) // change this
		return (1);
	args = parse_argv(argc, argv, envp);
	if (args == NULL)
		exit(EXIT_FAILURE);
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			return (perror("Pipe creation failed"), 1); // What error number do I need to return?
		pid = fork();
		if (pid == -1)
			return (perror("Fork creation failed"), 1);
		if (pid == 0)
			child_process(argc, &args[i - 2], envp, fd, i);
		else
			parent_process(fd, pid);
		i++;
	}
	return (free_struct(args), 0);
}
