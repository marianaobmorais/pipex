/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exec_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:15:00 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/23 18:01:32 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	**get_exec_args(char *argv)
{
	char	**args;
	char	*cmd_name;
	char	*new_arg;

	args = ft_split(argv, ' ');
	if (ft_strncmp(args[0], "/usr/bin/", 9) == 0 || ft_strncmp(args[0], "/bin/", 5) == 0)
	{
		cmd_name = ft_strrchr(args[0], '/') + 1;
		new_arg = ft_strdup(cmd_name);
		free(args[0]);
		args[0] = new_arg;
		return (args);
	}
	else
		return (args);
}
