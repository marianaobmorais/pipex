/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   studies3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 16:36:15 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/15 18:50:07 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <sys/wait.h> //incluir no header?

int	main(void)
{
	int	pid;
	int	err;
	int	wstatus;
	int	status_code;
	
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		// child process
		err = execlp("ping", "ping", "-c", "1", "google.com", NULL);
		if (err == -1)
			return (ft_printf("Error: no program to execute\n"), 2);
	}
	else
	{
		// parent process
		wait(&wstatus);
		if (WIFEXITED(wstatus))
		{
			status_code = WEXITSTATUS(wstatus);
			if (status_code == 0)
				ft_printf("Child was successful");
			else
				ft_printf("Child was not successful: status code: %d\n", status_code);
		}
		
		ft_printf("Ready to resume process\n");
	}
	return (0);
}