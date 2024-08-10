/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/10 20:17:43 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdio.h>

int	main(int ac, char **av)
{
/* 	if (!valid_args(ac, av))
		return (EXIT_FAILURE); */

	int	id;
	int	n;
	int	i;

	id = fork();
	if (id == 0)
		n = 1;
	else
		n = 6;
	if (id != 0)
		wait();
	i = n;
	while (i < n + 5)
	{
		ft_printf("%d ", i);
		fflush(stdout);
		i++;
	}
	if (id != 0)
		ft_printf("\n");
	return (0);
}
