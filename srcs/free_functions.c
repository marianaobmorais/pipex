/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 19:51:23 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/28 11:39:56 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/* typedef struct s_args
{
	char	**args;
	char	*pathname;
	int		fd_in;
	int		fd_out;
}	t_args; */

void	free_struct(t_args *args)
{
	int	i;

	if (args == NULL)
		return ;
	i = 0;
	while (args[i].args != NULL)
	{
		if (args[i].args != NULL)
			free_vector(args[i].args);
		if (args[i].pathname != NULL)
			free(args[i]. pathname);
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
