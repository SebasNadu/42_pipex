/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:13:39 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/14 01:11:23 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
	if ((err < 0 || err > 2) || param)
		pipex_perror(param, err);
	if (pipex->fd_in != -2)
		close(pipex->fd_in);
	if (pipex->fd_out != -2)
		close(pipex->fd_out);
	if (pipex->cmd_paths != NULL)
		free_array(pipex->cmd_paths, pipex->cmd_count);
	if (pipex->cmd_args != NULL)
		free_matrix(pipex->cmd_args, pipex->cmd_count);
	if (pipex->is_in_cpy)
		unlink(NO_INFILE);
	if (pipex->here_doc)
		unlink(HERE_DOC_PATH);
	if (pipex->is_urandom)
		unlink(URANDOM_PATH);
	free(pipex);
	exit(err);
	/*if (err > 1)*/
		/*exit(127);*/
	/*else if (err < 0)*/
		/*exit(2);*/
	/*else*/
		/*exit(err);*/
	return (0);
}

void	broken_pipe_perror(t_pipex *pipex, int i)
{
	if (i == 0)
		pipex_exit(pipex, pipex->cmd_args[i][0], CMD_NOT_FOUND);
	pipex_perror(pipex->cmd_args[i][0], CMD_NOT_FOUND);
	ft_putstr_fd(pipex->cmd_args[0][0], STDERR_FILENO);
	ft_putstr_fd(": stdout: Broken pipe\n", STDERR_FILENO);
	pipex_exit(pipex, NULL, 127);
}
