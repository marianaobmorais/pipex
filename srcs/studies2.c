/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   studies2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 18:49:48 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/15 18:50:23 by mariaoli         ###   ########.fr       */
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