/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/24 18:15:24 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

# include "../libft/libft.h"
# include <stdio.h> // perror
# include <string.h> //strerror
# include <wait.h>
# include <fcntl.h>
# include <errno.h>

typedef struct s_args
{
	char	**args;
	char	*pathname;
	int		fd_in;
	int		fd_out;
}	t_args;

t_args	*parse_argv(int argc, char **argv, char **envp);
void	free_struct(t_args *args);
void	free_vector(char **vector);
int		execute(t_args *args, char **envp);
void	child_process(int argc, t_args *args, char **envp, int *fd, int i);
void	parent_process(int *fd, pid_t pid);

#endif