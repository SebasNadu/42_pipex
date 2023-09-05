/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:05:13 by johnavar          #+#    #+#             */
/*   Updated: 2023/09/05 15:31:06 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	read_line(char **line, int fd, char delimiter)
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

void	here_doc_handler(char *arg)
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

void	urandom_handler(void)
{
	int		urandom_fd;
	int		tmp_fd;
	char	*buffer;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd < 0)
		return ;
	tmp_fd = open(URANDOM_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		return ;
	if (read_line(&buffer, urandom_fd, '\0') == -1)
		return ;
	write(tmp_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(tmp_fd);
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
		urandom_handler();
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

t_bool	get_outfile(t_pipex *pipex, char **argv, int argc)
{
	if (pipex->here_doc)
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->fd_out < 0)
		return (false);
	return (true);
}

t_bool	parse_args(int argc, char **argv, t_pipex *pipex)
{
	if (ft_strncmp(argv[1], "here_doc", 8) == 0 && ft_strlen(argv[1]) == 8)
		pipex->here_doc = true;
	else if (ft_strncmp(argv[1], "/dev/urandom", 12) == 0
		&& ft_strlen(argv[1]) == 12)
		pipex->is_urandom = true;
	if (argc < 5 + (int)pipex->here_doc)
		return (*(int *)pipex_exit(NULL, NULL, INV_ARGS));
	if (!pipex->here_doc && access(argv[1], F_OK) == -1)
		return (*(int *)pipex_exit(NULL, argv[1], NO_FILE));
	if (!pipex->here_doc && access(argv[1], R_OK) == -1)
		return (*(int *)pipex_exit(NULL, argv[1], NO_AUTH));
	if (get_infile(pipex, argv) == false)
		return (*(int *)pipex_exit(pipex, argv[1], NO_FILE));
	if (get_outfile(pipex, argv, argc) == false)
		return (*(int *)pipex_exit(pipex, argv[argc - 1], NO_FILE));
	pipex->cmd_count = argc - 3 - (int)pipex->here_doc;
	return (true);
}

char	**get_paths(char **envp, char *key)
{
	int		i;
	char	*path;

	i = -1;
	while (envp[++i])
		if (ft_strncmp(key, envp[i], ft_strlen(key)) == 0
			&& envp[i][ft_strlen(key)] == '=')
				path = ft_strchr(envp[i], '=') + 1;
	if (!path)
		return (NULL);
	return (ft_split(path, ':'));
}

char	*find_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*path;
	char	*full_path;

	if (access(cmd, F_OK) == 0)
		return (ft_substr(cmd, 0, ft_strlen(cmd)));
	paths = get_paths(envp, "PATH");
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], "/");
		if (!path)
			return (free_array(paths, -1), NULL);
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (!full_path)
			return (free_array(paths, -1), NULL);
		if (access(full_path, F_OK) == 0)
			return (free_array(paths, -1), full_path);
		free(full_path);
	}
	return (free_array(paths, -1), NULL);
}

t_bool	parse_paths(t_pipex *pipex, int argc, char **argv, char **envp)
{
	int		i;
	char	**cmd;

	pipex->cmd_paths = ft_calloc(pipex->cmd_count + 1, sizeof(char *));
	if (!pipex->cmd_paths)
		return (false);
	i = 1 + pipex->here_doc;
	while (++i < argc - 1)
	{
		cmd = ft_split(argv[i], ' ');
		if (!cmd)
			return (false);
		pipex->cmd_paths[i - 2 - pipex->here_doc] = find_cmd_path(cmd[0], envp);
		free_array(cmd, -1);
	}
	return (true);
}

void	init_pipex(t_pipex *pipex)
{
	pipex->fd_in = -1;
	pipex->fd_out = -1;
	pipex->here_doc = false;
	pipex->is_urandom = false;
	pipex->cmd_paths = NULL;
	pipex->cmd_args = NULL;
	pipex->cmd_count = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		return(*(int *)pipex_exit(NULL, NULL, NO_MEMORY));
	init_pipex(pipex);
	if (parse_args(argc, argv, pipex) == false)
		return (1);
	if (parse_paths(pipex, argc, argv, envp) == false)
		return (*(int *)pipex_exit(pipex, NULL, NO_PATH));
	return (*(int *)pipex_exit(pipex, NULL, END));
}
