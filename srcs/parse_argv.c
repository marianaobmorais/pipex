/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:37:15 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/24 20:04:44 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static char	*get_pathname(char **args, char **envp)
{
	char	*path_slash;
	char	*absolute_pathname;
	char	**paths;
	int		i;

	//ft_printf("args[0]: %s\n", args[0]); // erase
	if (!args[0])
		return (NULL); // permission denied: 
	if (access(args[0], F_OK) == 0 && access(args[0], X_OK) == 0)
		return (ft_strdup(args[0])); // do I need to allocate mem?
	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	paths = NULL;
	if (envp[i] != NULL)
		paths = ft_split(envp[i] + 5, ':');
	if (paths == NULL)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		path_slash = ft_strjoin(paths[i], "/");
		ft_printf("path_slash: %s\n", path_slash); // erase this
		absolute_pathname = ft_strjoin(path_slash, args[0]);
		free(path_slash);
		ft_printf("absolute path: %s\n", absolute_pathname); // erase this
		if (access(absolute_pathname, F_OK) == 0 && access(absolute_pathname, X_OK) == 0)
			return (free_vector(paths), absolute_pathname);
		free(absolute_pathname);
		i++;
	}
	ft_printf("path didnt work\n"); // erase this later
	return (free_vector(paths), NULL);
}

void	init(t_args *args, int argc)
{
	int	i;

	i = 0;
	while (i < argc - 2)
	{
		args[i].args = NULL;
		args[i].pathname = NULL;
		args[i].fd_in = -1;
		args[i].fd_out = -1;
		i++;
	}
}

t_args	*parse_argv(int argc, char **argv, char **envp)
{
	t_args	*args;
	int		i;

	args = (t_args *)malloc(sizeof(t_args) * (argc - 2));
	if (args == NULL)
		return (NULL);
	init(args, argc);
	i = 0;
	while (i < argc - 3)
	{
		args[i].fd_in = open(argv[1], O_RDONLY);
		if (args[i].fd_in == -1)
		{
			ft_printf("Error: permission denied: %s\n", argv[1]);
			//free_struct(args);
			//free(args);
			//return (free_struct(args), free(args), NULL);
		}
		args[i].fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644); // are the permissions right?
		if (args[i].fd_out == -1)
		{
			ft_printf("Error: permission denied: %s\n", argv[argc - 1]);
			//free_struct(args);
			//free(args);
			//return (free_struct(args), free(args), NULL);
		}
		args[i].args = ft_split(argv[i + 2], ' '); // get the args to be passed in the execve
		//if (args[i].args == NULL)
		//	free_struct(args);
		/* return (free_struct(args), NULL) */;
		args[i].pathname = get_pathname(args[i].args, envp); // check is the first split arg is executable
		if (args[i].pathname == NULL)
		{
			ft_printf("Error: Command not found: %s\n", args[i].args[0]);
			//free_struct(args); /* return (free_struct(args), NULL); */
		}
		i++;
	}
	args[i].args = NULL;
	args[i].pathname = NULL;
	//ft_printf("returning array of t_args\n");
	return (args);
}
