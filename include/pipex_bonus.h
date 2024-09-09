/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/09 15:40:54 by mariaoli         ###   ########.fr       */
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
# define ERR_ARGS_BONUS "Error: not enough arguments"
# define ERR_ARGS_HEREDOC "Error: not enough arguments for here_doc"
# define ERR_PERMISSION "Error: permission denied: "
# define ERR_MALLOC "Error: memory allocation failed: "
# define ERR_COMMAND "Error: command not found: "
# define ERR_FILE "Error: no such file or directory: "

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

int		validate_argc(int argc, bool is_heredoc);
t_args	*parse_argv(int argc, char **argv, char **envp);
bool	is_heredoc(char *argv);
int		heredoc_fd(char *limiter);
void	free_struct(t_args *args);
void	free_vector(char **vector);
void	exit_child(t_args *args, int *fd);
void	child_process(t_args *args, int i, char **envp, int *fd);
void	parent_process(int *fd, pid_t pid, bool is_heredoc);
void	ft_error(int fd, char *error_msg, char *filename);
void	ft_putstr_fd_len(char *s, int fd, int len);

#endif