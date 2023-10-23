/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:31:38 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/23 21:57:52 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

t_bool	parse_cmd_args(t_pipex *pipex, int argc, char **argv)
{
	int		i;
	char	**cmd;

	pipex->cmd_args = ft_calloc((argc - 2), sizeof(char **));
	if (!pipex->cmd_args)
		return (false);
	i = 1;
	while (++i < argc - 1)
	{
		cmd = split_with_quotes(argv[i], ' ');
		if (!cmd)
			return (false);
		pipex->cmd_args[i - 2] = cmd;
	}
	return (true);
}

void	cmd_cleaner(char *cmd)
{
	char	*tmp;
	int		i;
	int		j;

	tmp = ft_calloc(ft_strlen(cmd) + 1, sizeof(char));
	i = -1;
	j = -1;
	while (cmd[++i])
	{
		if (!ft_isalnum(cmd[i]) || !ft_isspace(cmd[i])
			|| cmd[i] == '/' || cmd[i] == '.')
			continue ;
		tmp[++j] = cmd[i];
	}
	if (access(tmp, F_OK) == 0)
	{
		ft_strlcpy(cmd, tmp, ft_strlen(tmp) + 1);
		free(tmp);
		return ;
	}
	free(tmp);
}

char	*path_creator(char **cmd)
{
	char	*path;
	int		i;

	path = ft_strdup(cmd[0]);
	i = 0;
	while (cmd[++i])
		path = ft_strjoin(path, cmd[i]);
	if (access(path, F_OK) == -1)
	{
		free(path);
		return (NULL);
	}
	return (path);
}

t_bool	parse_cmd_paths(t_pipex *pipex, int argc, char **argv, char **envp)
{
	int		i;
	char	**cmd;

	pipex->cmd_paths = ft_calloc(pipex->cmd_count + 1, sizeof(char *));
	if (!pipex->cmd_paths)
		return (false);
	i = 1;
	while (++i < argc -1)
	{
		if (access(argv[i], F_OK) == 0)
		{
			pipex->cmd_paths[i - 2] = ft_strdup(argv[i]);
			continue ;
		}
		cmd = split_with_quotes(argv[i], ' ');
		if (!cmd)
			return (false);
		pipex->cmd_paths[i - 2] = find_cmd_path(cmd[0], envp);
		if (pipex->cmd_paths[i - 2] == NULL)
			pipex->cmd_paths[i - 2] = ft_strjoin("/usr/bin/", cmd[0]);
		free_array(cmd, -1);
	}
	return (true);
}

t_bool	parse_args(int argc, char **argv, t_pipex *pipex)
{
	if (ft_strncmp(argv[1], "/dev/urandom", 12) == 0
		&& ft_strlen(argv[1]) == 12)
		pipex->is_urandom = true;
	if (get_infile(pipex, argv) == false)
		pipex_perror(argv[1], NO_FILE);
	if (access(argv[argc - 1], F_OK) == -1)
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	else if (access(argv[argc - 1], R_OK) == -1)
		pipex_exit(pipex, argv[argc - 1], NO_WRITE);
	else
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (pipex->fd_out < 0)
		return (*(int *)pipex_exit(pipex, argv[argc - 1], NO_FILE));
	pipex->cmd_count = argc - 3;
	return (true);
}
