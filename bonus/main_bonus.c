/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/30 18:26:59 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	t_args	*args;
	int		fd[2];
	int		i;

	if (argc < 5)
		return (ft_printf(ERR_ARGS_BONUS), 2);
	args = parse_argv(argc, argv, envp);
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			return (perror(PERR_PIPE), 1);
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
	return (free_struct(args, argc), 0);
}
