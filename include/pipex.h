/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:48:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/19 19:52:00 by mariaoli         ###   ########.fr       */
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

#endif