/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:44:47 by mariaoli          #+#    #+#             */
/*   Updated: 2024/08/28 20:29:31 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

t_open	open_files(char *infile, char *outfile)
{
	t_open fd;

	fd.infile = ft_strdup(infile); // do I need to check if fd.infile == NULL?
	if (access(fd.infile, F_OK) == -1) // checks existence of infile
	{
		ft_printf("%s: no such file or directory\n", infile);
		fd.fd_in = open("/dev/null", O_RDONLY); // fd_in is not -1
	}
	else
	{
		fd.fd_in = open(infile, O_RDONLY);
		if (fd.fd_in == -1) // checks readability of infile
		{
			ft_printf("%s: no such file or directory\n", infile); // this is for when "chmod 000 infile"
			fd.fd_in = open("/dev/null", O_RDONLY); // fd_in is no longer -1
		}
	}
	fd.fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); // are the permissions right?
	if (fd.fd_out == -1)
		ft_printf("Error: permission denied: %s\n", outfile);
	fd.fd[0] = -1;
	fd.fd[1] = -1;
	return (fd);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	t_args	*args;
	t_open	fd;
	int		i;

	if (argc < 5) // change this
		return (1);
	args = parse_argv(argc, argv, envp);
	fd = open_files(argv[1], argv[argc - 1]);
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd.fd) == -1)
			return (perror("Pipe creation failed"), 1); // What error number do I need to return?
		pid = fork();
		if (pid == -1)
			return (perror("Fork creation failed"), 1);
		if (pid == 0)
			child_process(args, i - 2, envp, fd); // passar o fd_files por referencia para poder dar free mais na frente?
		else
			parent_process(fd.fd);
		i++;
	}
	if (pid != 0)
		wait(NULL);
	return (free(fd.infile), free_struct(args), 0);
}
