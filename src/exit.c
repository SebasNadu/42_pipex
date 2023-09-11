/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:13:39 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/09/11 23:27:30 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	pipex_perror(char *param, int err)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	if (param && (err == CMD_NOT_FOUND || err == NO_FILE || err == NO_AUTH
			|| err == CMD_FAIL))
		ft_putstr_fd(param, STDERR_FILENO);
	if (err == CMD_NOT_FOUND)
		ft_putstr_fd(": command not found", STDERR_FILENO);
	else if (err == NO_FILE)
		ft_putstr_fd("no such file or directory: ", STDERR_FILENO);
	else if (err == NO_AUTH)
		ft_putstr_fd("permission denied: ", STDERR_FILENO);
	else if (err == CMD_FAIL)
		ft_putstr_fd(": command failed", STDERR_FILENO);
	else if (err == INV_ARGS)
		ft_putstr_fd("invalid arguments: ", STDERR_FILENO);
	else if (err == NO_MEMORY)
		ft_putstr_fd("no memory left on device: ", STDERR_FILENO);
	else if (err == DUP_ERR)
		ft_putstr_fd("could not duplicate the fd: ", STDERR_FILENO);
	else if (err == PIPE_ERR)
		ft_putstr_fd("could not create the pipe: ", STDERR_FILENO);
	else if (err == FORK_ERR)
		ft_putstr_fd("could not create the child process: ", STDERR_FILENO);
	else if (err == NO_PATH)
		ft_putstr_fd("PATH variable is not set ", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	free_array(char **array, int size)
{
	int	i;

	i = -1;
	if (size == -1)
		while (array[++i])
			free(array[i]);
	else
		while (++i < size)
			if (array[i])
				free(array[i]);
	free(array);
}

void	free_matrix(char ***matrix, int size)
{
	int	i;

	i = -1;
	if (size == -1)
		while (matrix[++i])
			free_array(matrix[i], -1);
	else
		while (++i < size)
			if (matrix[i])
				free_array(matrix[i], -1);
	free(matrix);
}

void	*pipex_exit(t_pipex *pipex, char *param, int err)
{
	if (err < 1 || param)
		pipex_perror(param, err);
	if (pipex->fd_in != -2)
		close(pipex->fd_in);
	if (pipex->fd_out != -2)
		close(pipex->fd_out);
	if (pipex->cmd_paths != NULL)
		free_array(pipex->cmd_paths, pipex->cmd_count);
	if (pipex->cmd_args != NULL)
		free_matrix(pipex->cmd_args, pipex->cmd_count);
	if (pipex->here_doc)
		unlink(HERE_DOC_PATH);
	if (pipex->is_urandom)
		unlink(URANDOM_PATH);
	free(pipex);
	if (err > 1)
		exit(err);
	else if (err < 0)
		exit(2);
	else
		exit(EXIT_SUCCESS);
	return (0);
}
