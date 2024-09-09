/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/09 15:45:34 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdbool.h>

# define PERR_PIPE "Error: pipe creation failed"
# define PERR_FORK "Error: fork creation failed"
# define PERR_EXECVE "Error: execve"
# define ERR_ARGS "Error: required usage: ./pipex file1 cmd1 cmd2 file2"
# define ERR_PERMISSION "Error: permission denied: "
# define ERR_MALLOC "Error: memory allocation failed: "
# define ERR_COMMAND "Error: command not found: "
# define ERR_FILE "Error: no such file or directory: "

typedef struct s_args
{
	char	**args;
	char	*pathname;
	bool	first_child;
	bool	last_child;
	char	*infile;
	char	*outfile;
	int		argc;
}	t_args;

t_args	*parse_argv(int argc, char **argv, char **envp);
void	free_struct(t_args *args);
void	free_vector(char **vector);
void	exit_child(t_args *args, int *fd);
void	child_process(t_args *args, int i, char **envp, int *fd);
void	parent_process(int *fd);
void	ft_error(int fd, char *error_msg, char *filename);
void	ft_putstr_fd_len(char *s, int fd, int len);

#endif