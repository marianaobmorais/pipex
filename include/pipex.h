/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/20 17:36:23 by mariaoli         ###   ########.fr       */
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

void	free_vector(char **vector);
char	*get_pathname(char *argv, char **envp);
char	*get_first_word(char *s, int c);
char	**get_exec_args(char *argv);
int		first_child(int *fd, char **av, char **envp);
int		last_child(int *fd, int ac, char **av, char **envp);

#endif