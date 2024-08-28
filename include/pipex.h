/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/28 19:59:33 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

# include "../libft/libft.h"
# include <stdio.h> // perror
# include <string.h> //strerror
# include <wait.h>
# include <fcntl.h>
//# include <errno.h> // do I need this one?
# include <stdbool.h>

typedef struct s_open
{
	char	*infile;
	int		fd_in;
	int		fd_out;
	int		fd[2];
}	t_open;

typedef struct s_args
{
	char	**args;
	char	*pathname;
	bool	first_child;
	bool	last_child;
}	t_args;

t_args	*parse_argv(int argc, char **argv, char **envp);
void	free_struct(t_args *args);
void	free_vector(char **vector);
void	child_process(t_args *args, int i, char **envp, t_open fd);
void	parent_process(int *fd);


// int		execute(t_args *args, char **envp);

#endif