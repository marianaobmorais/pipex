/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 18:07:31 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/03 19:52:44 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static int	read_heredoc(char *limiter)
{
	int		fd_write;
	int		fd_read;
	char	*input;
	char	*limiter_nl;

	fd_write = open(".tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	input = (char *)malloc(sizeof(char));
	*input = '\0';
	while (input != NULL)
	{
		ft_printf("pipe heredoc> ");
		input = get_next_line(STDIN_FILENO);
		limiter_nl = ft_strjoin(limiter, "\n");
		if (ft_strncmp(limiter_nl, input, ft_strlen(input)) == 0)
		{
			free(limiter_nl);
			free(input);
			break ;
		}
		free(limiter_nl);
		if (write(fd_write, input, ft_strlen(input)) == -1)
			return (free(input), -1);
		free(input);
	}
	return (fd_read = open(".tmp", O_RDONLY));
}

int	heredoc_fd(char *limiter)
{
	int	fd;

	fd = read_heredoc(limiter);
	unlink(".tmp");
	return (fd);
}

bool	is_heredoc(char *argv)
{
	if (ft_strncmp(argv, "here_doc", 8) == 0)
		return (true);
	return (false);
}
