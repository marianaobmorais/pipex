/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/30 10:11:51 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	t_args	*args;
	int		fd[2];
	int		i;

	if (argc < 5) // change this for mandatory part
		return (ft_printf(ERR_ARGS_BONUS), 1);
	args = parse_argv(argc, argv, envp);
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			return (perror(PERR_PIPE), 1); // What error number do I need to return?
		pid = fork();
		if (pid == -1)
			return (perror(PERR_FORK), 1);
		if (pid == 0)
			child_process(args, i - 2, envp, fd);
		else
			parent_process(fd);
		i++;
	}
	if (pid != 0)
		wait(NULL);
	return (free_struct(args), 0);
}
