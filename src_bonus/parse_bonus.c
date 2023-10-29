/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:09:49 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/23 21:58:00 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

t_bool	parse_cmd_args(t_pipex *pipex, int argc, char **argv)
{
	int		i;
	char	**cmd;

	pipex->cmd_args = ft_calloc((argc - 2 - pipex->here_doc), sizeof(char **));
	if (!pipex->cmd_args)
		return (false);
	i = 1 + pipex->here_doc;
	while (++i < argc - 1)
	{
		cmd = split_with_quotes(argv[i], ' ');
		if (!cmd)
			return (false);
		pipex->cmd_args[i - 2 - pipex->here_doc] = cmd;
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
	i = 1 + pipex->here_doc;
	while (++i < argc - 1)
	{
		if (access(argv[i], F_OK) == 0)
		{
			pipex->cmd_paths[i - 2] = ft_strdup(argv[i]);
			continue ;
		}
		cmd = split_with_quotes(argv[i], ' ');
		if (!cmd)
			return (false);
		pipex->cmd_paths[i - 2 - pipex->here_doc] = find_cmd_path(cmd[0], envp);
		if (pipex->cmd_paths[i - 2 - pipex->here_doc] == NULL)
			pipex->cmd_paths[i - 2 - pipex->here_doc]
				= ft_strjoin("/usr/bin/", cmd[0]);
		free_array(cmd, -1);
	}
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
		return (*(int *)pipex_exit(pipex, NULL, INV_ARGS));
	if (get_infile(pipex, argv) == false)
		pipex_perror(argv[1], NO_FILE);
	if (get_outfile(pipex, argv, argc) == false)
		return (*(int *)pipex_exit(pipex, argv[argc - 1], NO_FILE));
	else if (access(argv[argc - 1], R_OK) == -1)
		pipex_exit(pipex, argv[argc - 1], NO_WRITE);
	pipex->cmd_count = argc - 3 - (int)pipex->here_doc;
	return (true);
}
