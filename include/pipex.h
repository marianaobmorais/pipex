/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/28 12:01:35 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

# include "../libft/libft.h"
# include <stdio.h> // perror
# include <string.h> //strerror
//# include <wait.h>
# include <fcntl.h>
# include <errno.h> // do I need this one?
# include <stdbool.h>

typedef struct s_open
{
	char	*infile;
	int		fd_in;
	int		fd_out;
}	t_open;

typedef struct s_args
{
	char	**args;
	char	*pathname;
	bool	first_child;
	bool	last_child;
}	t_args;

/* typedef struct s_args
{
	char	**args;
	char	*pathname;
	int		fd_in;
	int		fd_out;
}	t_args; */

t_args	*parse_argv(int argc, char **argv, char **envp);
void	free_struct(t_args *args);
void	free_vector(char **vector);
// int		execute(t_args *args, char **envp);
// void	child_process(int argc, t_args *args, char **envp, int *fd, int i);
// void	parent_process(int *fd, pid_t pid);

#endif