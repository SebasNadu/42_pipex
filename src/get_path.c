/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 10:36:30 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/29 21:42:18 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static char	**get_paths(char **envp, char *key)
{
	int		i;
	char	*path;
	char	**paths;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			path = ft_strchr(envp[i], '=');
			if (!path)
				return (NULL);
			path++;
			paths = ft_split(path, ':');
			if (!paths)
				return (NULL);
			return (paths);
		}
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**paths;
	char	*full_path;

	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
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
