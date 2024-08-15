/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   studies1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/15 18:50:51 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdio.h>
#include <sys/wait.h>

//The child process goes first and should end first

int	main(int ac, char **av)
{
	int	fd[2];
	// two different fd for two different ends of a pipe
	// fd[0] - read end
	// fd[1] - write end
	int	id;
	int	x;
	int	y;

	if (pipe(fd) == -1)
		return (ft_printf("Error occured while opening the pipe\n"), 1);
	id = fork(); //the fd declared in the beginning are copied over - they are inherited by processes created in fork
	if (id == -1)
		return (ft_printf("Error occured with the fork\n"), 4);
	if (id == 0) // This is for the child process. The child process is the first to run
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
		printf("Number got from child process: %d\n", y);
	}
	
	return (0);
}