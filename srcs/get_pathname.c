/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pathname.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 21:27:17 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/23 19:05:17 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/* This function extracts the first word from the string s up to 
the first occurrence of the character c. It allocates memory for 
the first word and returns it. */

char	*get_first_word(char *s, int c)
{
	char	*first_word;
	int		i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
		i++;
	first_word = (char *)malloc(sizeof(char) * (i + 1));
	if (first_word == NULL)
		return (NULL);
	i = 0;
	while (s[i] != c && s[i] != '\0')
	{
		first_word[i] = s[i];
		i++;
	}
	first_word[i] = '\0';
	return (first_word);
}

/* This function aims to retrieve the executable path based 
on the argv input and the environment variables (envp). 
It uses get_first_word to extract the command name from argv. 
Then it checks whether the command is an absolute path (starting with /). 
If it is, it checks for the command's existence and returns it if found. 
Otherwise, it searches for the command in the directories specified by the 
PATH environment variable. */

char	*get_pathname(char *arg, char **envp)
{
	char	*pathname;
	char	*slash_pathname;
	char	*absolute_pathname;
	char	**paths;

	pathname = get_first_word(arg, ' ');
	if (pathname == NULL)
		return (NULL);
	if (ft_strncmp(pathname, "/", 1) == 0)
	{
		if (access(pathname, F_OK) == -1)
		{
			perror(pathname);
			free(pathname);
			return (NULL);
		}
		return (pathname);
	}
	else
	{
		int i = 0;
		while (envp[i] != NULL)
		{
			if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			{
				paths = ft_split(envp[i] + 5, ':');
				slash_pathname = ft_strjoin("/", pathname);
				int j = 0;
				while (paths[j] != NULL)
				{
					absolute_pathname = ft_strjoin(paths[j], slash_pathname);
					if (access(absolute_pathname, F_OK) == 0 && access(absolute_pathname, X_OK) == 0)
					{
						free_vector(paths);
						free(slash_pathname);
						free(pathname);
						return (absolute_pathname);
					}
					free(absolute_pathname);
					j++;
				}
			}
			i++;
		}
		if (access(pathname, F_OK) == -1 || access(absolute_pathname, X_OK) == -1) 
		{
			perror(pathname); // How to do it so it prints correctly? like < infile /usr/bn/ls -l | wc -l > outfile prints zsh: no such file or directory: /usr/bn/ls
			free_vector(paths);
			free(slash_pathname);
			free(pathname);
			return (NULL);
		}
	}
	return (pathname);
}
