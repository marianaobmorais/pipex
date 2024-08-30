/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:51:23 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/30 12:22:35 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/* void	free_struct(t_args *args, int count) // count = argc - 3?
{
	int	i;

	i = 0;
	if (args == NULL)
		return ;
	while (i <= count)
	{
		if (args[i].args != NULL)
			free_vector(args[i].args);
		if (args[i].pathname != NULL)
			free(args[i].pathname);
		if (args[i].infile != NULL)
			free(args[i].infile);
		if (args[i].outfile != NULL)
			free(args[i].outfile);
		i++;
	}
	free(args);
} */

void	free_struct(t_args *args)
{
	int	i;

	if (args == NULL)
		return ;
	i = 0;
	while (args[i].args != NULL)
	{
		free_vector(args[i].args);
		if (args[i].pathname != NULL)
			free(args[i].pathname);
		if (args[i].infile != NULL)
			free(args[i].infile);
		if (args[i].outfile != NULL)
			free(args[i].outfile);
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
