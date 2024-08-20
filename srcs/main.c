/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/20 17:42:51 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	main(int ac, char **av , char **envp)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (ac != 5)
		return (1);
	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == -1)
		return (perror("Error during fork"), 1); // What number do I need to return?
	if (pid1 == 0)
		first_child(fd, av, envp);
	pid2 = fork();
	if (pid2 == -1)
		return (perror("Error during fork"), 1);
	if (pid2 == 0)
		last_child(fd, ac, av, envp);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
