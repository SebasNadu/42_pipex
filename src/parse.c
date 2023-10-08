/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:31:38 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/09 00:57:04 by sebasnadu        ###   ########.fr       */
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
	pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->fd_out < 0)
		return (*(int *)pipex_exit(pipex, argv[argc - 1], NO_FILE));
	pipex->cmd_count = argc - 3;
	return (true);
}
