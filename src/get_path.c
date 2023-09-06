/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:19:33 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/09/05 16:25:11 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static char	**get_paths(char **envp, char *key)
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
