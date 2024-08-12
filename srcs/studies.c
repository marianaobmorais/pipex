/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   studies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/12 17:53:15 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdio.h>
#include <sys/wait.h>

/* int fork1(void)
{
	int pid;

	pid = fork();
	if (pid == -1)
		exit(1);
	return (pid);
} */

int	main(int ac, char **av)
{
	int	arr[] = {1, 2, 3, 4, 1, 2, 7};
	int	arr_size = sizeof(arr) / sizeof(int);
	int	start;
	int	end;
	int	fd[2];
	int	id;
	int	i;
	int	sum;
	int	sum_child;
	int	total_sum;

	if (pipe(fd) == -1)
		return (1);
	id = fork();
/* 	if (fork1() == 0)
		// child
	// parent */
	if (id == -1)
		return (2);
	if (id == 0) // child process
	{
		start = 0;
		end = arr_size / 2;
	}
	else // parent process
	{
		start = arr_size / 2;
		end = arr_size;
	}
	sum = 0;
	i = start;
	while (i < end)
	{
		sum += arr[i];
		i++;
	}
	ft_printf("Calculated partial sum: %d\n", sum);
	if (id == 0) //send partial sum from child to parent
	{
		close (fd[0]);
		if (write(fd[1], &sum, sizeof(sum)) == -1) // sizeof(int) also works
			return (1);
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		if (read(fd[0], &sum_child, sizeof(sum_child)) == -1)
			return (1);
		close(fd[0]);
		total_sum = sum + sum_child;
		ft_printf("Total sum is: %d\n", total_sum);
		wait(NULL); // wait for the child process to finish before starting parent process
	}
	return (0);
}


//The child process goes first and should end first

/* int	main(int ac, char **av)
{
	// if (!valid_args(ac, av))
	// 	return (EXIT_FAILURE);

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
} */