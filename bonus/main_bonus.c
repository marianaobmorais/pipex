/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/06 20:53:36 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

int	validate_argc(int argc, bool is_heredoc)
{
	if (argc >= 5)
	{
		if (is_heredoc == true && argc < 6)
			return (ft_printf(ERR_ARGS_HEREDOC), -1);
		return (0);
	}
	return (ft_printf(ERR_ARGS_BONUS), -1);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	t_args	*args;
	int		fd[2];
	int		i;

	if (validate_argc(argc, is_heredoc(argv[1])))
		return (2);
	args = parse_argv(argc, argv, envp);
	i = 2 + is_heredoc(argv[1]);
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			return (perror(PERR_PIPE), 1);
		pid = fork();
		if (pid == -1)
			return (perror(PERR_FORK), 1);
		if (pid == 0)
			child_process(args, i - 2 - is_heredoc(argv[1]), envp, fd);
		else
			parent_process(fd, pid, is_heredoc(argv[1]));
		i++;
	}
	if (pid != 0)
		wait(NULL);
	return (free_struct(args), 0);
}
