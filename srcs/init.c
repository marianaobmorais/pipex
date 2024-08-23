/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:37:15 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/23 16:41:07 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	**vector_dup(int argc, char **argv)
{
	char	**res;
	int		i;

	res = (char **)malloc(sizeof(char *) * (argc + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (argv[i] != NULL)
	{
		res[i] = ft_strdup(argv[i]);
		if (res[i] == NULL)
		{
			i = 0;
			free_vector(res);
			return (NULL);
		}
		i++;
	}
	res[i] = NULL;
	return (res);
}

t_args	*init(int argc, char **argv)
{
	t_args	*args;

	args = (t_args *)malloc(sizeof(t_args));
	if (args == NULL)
		return (NULL);
	args->argc = argc;
	args->argv = vector_dup(argc, argv);
	if (args->argv == NULL)
		return (free(args), NULL);
	args->fd_in = open(argv[1], O_RDONLY);
	if (args->fd_in == -1)
		return (free(args), free_vector(args->argv), NULL);
	args->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644); // check the permissions
	if (args->fd_out == -1)
		return (free(args), free_vector(args->argv), NULL);
	return (args);
}
