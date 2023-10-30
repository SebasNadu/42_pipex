/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:42:14 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/30 10:15:26 by sebas_nadu       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static int	read_line(char **line, int fd, char delimiter)
{
	char	*buffer;
	char	c;
	int		i;

	i = 0;
	if (fd < 0)
		return (-1);
	buffer = ft_calloc(10000, sizeof(char));
	if (!buffer)
		return (-1);
	while (read(fd, &c, 1) > 0 && c && c != delimiter && i < 10000 - 2)
		buffer[i++] = c;
	buffer[i++] = '\n';
	buffer[i] = '\0';
	*line = buffer;
	return (i);
}

static t_bool	urandom_handler(void)
{
	char	*buffer;
	int		urandom_fd;
	int		tmp_fd;
	int		i;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
		return (false);
	tmp_fd = open(URANDOM_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
		return (false);
	i = -1;
	while (++i < 20)
	{
		if (read_line(&buffer, urandom_fd, '\n') == -1)
			return (false);
		write(tmp_fd, buffer, ft_strlen(buffer));
		free(buffer);
	}
	close(tmp_fd);
	close(urandom_fd);
	return (true);
}

t_bool	get_infile(t_pipex *pipex, char **argv)
{
	if (pipex->is_urandom == true)
	{
		if (!urandom_handler())
			return (false);
		pipex->fd_in = open(URANDOM_PATH, O_RDONLY);
	}
	else
	{
		if (access(argv[1], F_OK) == -1 || access(argv[1], R_OK) == -1)
		{
			if (access(argv[1], R_OK) == -1)
				pipex_perror(argv[1], NO_READ);
			pipex->is_in_cpy = true;
			pipex->fd_in = open(NO_INFILE, O_RDONLY | O_CREAT, 0644);
			return (true);
		}
		else
			pipex->fd_in = open(argv[1], O_RDONLY);
	}
	return (true);
}
