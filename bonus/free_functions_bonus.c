/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:51:23 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/01 00:32:00 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	free_struct(t_args *args, int argc)
{
	int	i;

	if (args == NULL)
		return ;
	i = 0;
	while (i <= argc - 3)
	{
		if (args[i].args != NULL)
			free_vector(args[i].args);
		if (args[i].pathname != NULL)
			free(args[i].pathname);
		// if (args[i].infile != NULL)
		// 	free(args[i].infile);
		// if (args[i].outfile != NULL)
		// 	free(args[i].outfile);
		i++;
	}
	free(args);
}

void	free_vector(char **vector)
{
	int	i;

	i = 0;
	if (vector != NULL)
	{
		while (vector[i] != NULL)
		{
			free(vector[i]);
			i++;
		}
		free(vector);
	}
}
