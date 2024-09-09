/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:37:15 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/09 14:10:39 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void	init_args(t_args *args, int argc, bool is_heredoc)
{
	int	count;

	count = 0;
	while (count <= argc - 3 - is_heredoc)
	{
		args[count].infile = NULL;
		args[count].outfile = NULL;
		args[count].argc = 0;
		args[count].limiter = NULL;
		args[count].args = NULL;
		args[count].pathname = NULL;
		args[count].first_child = false;
		args[count].last_child = false;
		args[count].heredoc = is_heredoc;
		count++;
	}
}

static char	*iterate_env_path(char **paths, char *arg)
{
	char	*path_slash;
	char	*absolute_pathname;
	int		i;

	i = 0;
	while (paths[i] != NULL)
	{
		path_slash = ft_strjoin(paths[i], "/");
		absolute_pathname = ft_strjoin(path_slash, arg);
		free(path_slash);
		if (access(absolute_pathname, F_OK) == 0
			&& access(absolute_pathname, X_OK) == 0)
			return (absolute_pathname);
		free(absolute_pathname);
		i++;
	}
	return (NULL);
}

static char	*get_pathname(char **args, char **envp)
{
	char	*absolute_pathname;
	char	**paths;
	int		i;

	if (args[0] == NULL)
		return (ft_strdup(args[0]));
	if (access(args[0], F_OK) == 0 && access(args[0], X_OK) == 0)
		return (ft_strdup(args[0]));
	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	paths = NULL;
	if (envp[i] != NULL)
	{
		paths = ft_split(envp[i] + 5, ':');
		if (paths == NULL)
			return (ft_fprintf(2, ERR_MALLOC, "get_pathname"), NULL);
	}
	if (paths == NULL)
		return (ft_strdup(args[0]));
	absolute_pathname = iterate_env_path(paths, args[0]);
	free_vector(paths);
	if (absolute_pathname != NULL)
		return (absolute_pathname);
	return (ft_strdup(args[0]));
}

static void	error_malloc(t_args *args)
{
	if (args != NULL)
	{
		ft_fprintf(2, ERR_MALLOC, "ft_split in parse_argv");
		free(args);
	}
	else
		ft_fprintf(2, ERR_MALLOC, "parse_argv");
	exit(EXIT_FAILURE);
}

t_args	*parse_argv(int argc, char **argv, char **envp)
{
	t_args	*args;
	int		count;

	args = (t_args *)malloc(sizeof(t_args) * (argc - 2));
	if (args == NULL)
		error_malloc(args);
	init_args(args, argc, is_heredoc(argv[1]));
	count = 0;
	while (count < argc - 3 - is_heredoc(argv[1]))
	{
		args[count].infile = argv[1];
		args[count].outfile = argv[argc - 1];
		args[count].argc = argc;
		args[count].limiter = argv[2];
		args[count].args = ft_split(argv[count + 2 + is_heredoc(argv[1])], ' ');
		if (args[count].args == NULL)
			error_malloc(args);
		args[count].pathname = get_pathname(args[count].args, envp);
		if (count == 0)
			args[count].first_child = true;
		if (count == argc - 4 - is_heredoc(argv[1]))
			args[count].last_child = true;
		count++;
	}
	return (args);
}
