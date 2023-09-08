/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_io_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:16:03 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/09/08 09:50:10 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

t_bool	get_outfile(t_pipex *pipex, char **argv, int argc)
{
	if (pipex->here_doc)
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT
				| O_APPEND, 0644);
	else
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT
				| O_TRUNC, 0644);
	if (pipex->fd_out < 0)
		return (false);
	return (true);
}

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

static void	here_doc_handler(char *arg)
{
	int		fd;
	char	*buffer;

	fd = open(HERE_DOC_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
	write(1, "heredoc> ", 9);
	while (fd > 0 && read_line(&buffer, STDIN_FILENO, '\n') > 0)
	{
		if (ft_strncmp(buffer, arg, ft_strlen(arg)) == 0
			&& ft_strlen(buffer) == ft_strlen(arg) + 1)
		{
			free(buffer);
			break ;
		}
		write(fd, buffer, ft_strlen(buffer));
		free(buffer);
		write(1, "heredoc> ", 9);
	}
	close(fd);
}

static t_bool	urandom_handler(void)
{
	int		urandom_fd;
	int		tmp_fd;
	char	*buffer;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd < 0)
		return (false);
	tmp_fd = open(URANDOM_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		return (false);
	if (read_line(&buffer, urandom_fd, '\0') == -1)
		return (false);
	write(tmp_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(tmp_fd);
	close(urandom_fd);
	return (true);
}

t_bool	get_infile(t_pipex *pipex, char **argv)
{
	if (pipex->here_doc)
	{
		here_doc_handler(argv[2]);
		pipex->fd_in = open(HERE_DOC_PATH, O_RDONLY);
	}
	else if (pipex->is_urandom)
	{
		if (urandom_handler() == false)
			return (false);
		pipex->fd_in = open(URANDOM_PATH, O_RDONLY);
	}
	else
	{
		if (access(argv[1], F_OK) == -1)
			return (false);
		else
			pipex->fd_in = open(argv[1], O_RDONLY);
	}
	if (pipex->fd_in < 0)
		return (false);
	return (true);
}
