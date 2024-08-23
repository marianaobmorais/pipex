/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/23 18:33:11 by mariaoli         ###   ########.fr       */
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
	char	argc;
	char	**argv;
	int		fd_in;
	int		fd_out;
}	t_args;

t_args	*init(int argc, char **argv);
char	**vector_dup(int argc, char **argv);
void	child_process(t_args *args, char **envp, int *fd, int i);
int		execute(char *argv, char **envp);
void	parent_process(int *fd, pid_t pid);
void	free_vector(char **vector);
char	*get_pathname(char *argv, char **envp);
char	*get_first_word(char *s, int c);
char	**get_exec_args(char *argv);
char	**vector_dup(int argc, char **argv);

#endif