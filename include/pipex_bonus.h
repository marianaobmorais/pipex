/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/01 16:59:23 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdbool.h>

# define PERR_PIPE "Error: pipe creation failed"
# define PERR_FORK "Error: fork creation failed"
# define PERR_EXECVE "Error: execve"
# define ERR_ARGS_BONUS "Error: not enough arguments\n"
# define ERR_PERMISSION "Error: permission denied: %s\n"
# define ERR_MALLOC "Error: memory allocation failed: %s\n"
# define ERR_COMMAND "Error: command not found: %s\n"
# define ERR_FILE "Error: no such file or directory: %s\n"

typedef struct s_open
{
	char	*infile;
	int		fd_in;
	int		fd_out;
	int		fd[2];
}	t_open;

typedef struct s_args
{
	char	*infile;
	char	*outfile;
	int		argc;
	char	*limiter;
	char	**args;
	char	*pathname;
	bool	first_child;
	bool	last_child;
	bool	heredoc;
}	t_args;

t_args	*parse_argv(int argc, char **argv, char **envp);
//int		heredoc_offset(char *argv);
bool	is_heredoc(char *argv);
void	free_struct(t_args *args, int count);
void	free_vector(char **vector);
void	child_process(t_args *args, int i, char **envp, int *fd);
void	parent_process(int *fd, pid_t pid, bool is_heredoc);

#endif