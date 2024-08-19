/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   studies4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 18:51:21 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/15 19:54:09 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <time.h>
#include <sys/wait.h> //incluir no header?

int	main(void)
{
	int	fd[2];
	int	pid;
	int	arr[10];
	int	n;
	int	i;
	int	sum;

	if (pipe(fd) == -1) // open pipe before fork, so both child and parent get the pipe
		return (1);
	pid = fork();
	if (pid == -1)
		return (2);
	if (pid == 0) // child process
	{
		close(fd[0]); // close the read end
		srand(time(NULL));
		n = (rand() % 10) + 1;
		i = 0;
		while (i < n)
		{
			arr[i] = rand() % 11;
			i++;
		}
		if (write(fd[1], &n, sizeof(int)) < 0) // send n
			return (3);
		if (write(fd[1], arr, sizeof(int) * n) < 0) // send array
			return (4);
		close(fd[1]);
	}
	else // parent process
	{
		close(fd[1]); // close write end
		if (read(fd[0], &n, sizeof(int)) < 0)
			return (5);
		if (read(fd[0], arr, sizeof(int) * n) < 0)
			return (6);
		close(fd[0]);
		sum = 0;
		i = 0;
		while (i < n)
		{
			sum += arr[i];
			i++;
		}
		ft_printf("Result: %d\n", sum);
		wait(NULL); // always wait for the child process to finish
	}
}