/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:05:13 by johnavar          #+#    #+#             */
/*   Updated: 2023/09/04 12:34:56 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <unistd.h>

void	pipex_perror(char *param, int err)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	if (err == CMD_NOT_FOUND)
		ft_putstr_fd("command not found: ", STDERR_FILENO);
	else if (err == NO_FILE)
		ft_putstr_fd("no such file or directory: ", STDERR_FILENO);
	else if (err == NO_AUTH)
		ft_putstr_fd("permission denied: ", STDERR_FILENO);
	else if (err == CMD_FAIL)
		ft_putstr_fd("command failed: ", STDERR_FILENO);
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
	if (param && (err == CMD_NOT_FOUND || err == NO_FILE || err == NO_AUTH ||
		err == CMD_FAIL))
		ft_putstr_fd(param, STDERR_FILENO);
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

void free_matrix(char ***matrix, int size)
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
	if (pipex->fd_in != -1)
		close(pipex->fd_in);
	if (pipex->fd_out != -1)
		close(pipex->fd_out);
	if (pipex->cmd_paths != NULL)
		free_array(pipex->cmd_paths, pipex->cmd_count);
	if (pipex->cmd_args != NULL)
		free_matrix(pipex->cmd_args, pipex->cmd_count);
	if (pipex->here_doc)
		unlink(HERE_DOC_PATH);
	if (pipex->is_urandom)
		unlink(URANDOM_PATH);
	if (pipex->is_invalid_input)
		unlink(INVALID_INPUT_PATH);
	free(pipex);
	exit(0);
	return (0);
}

t_bool	parse_args(int argc, char **argv, t_pipex *pipex)
{
	if (ft_strncmp(argv[1], "here_doc", 8) == 0 && ft_strlen(argv[1]) == 8)
		pipex->here_doc = true;
	else if (ft_strncmp(argv[1], "/dev/urandom", 12) == 0
		&& ft_strlen(argv[1]) == 12)
		pipex->is_urandom = true;
	if (argc < 5 + (int)pipex->here_doc)
		return (false);
	if (!pipex->here_doc && access(argv[1], F_OK) == -1)
		return (NO_FILE);
	if (!pipex->here_doc && access(argv[1], R_OK) == -1)
		return (NO_AUTH);
	if (parse_args(argc, argv, pipex))
		return (INV_ARGS);
	return (true);
}

void	init_pipex(t_pipex *pipex)
{
	pipex->fd_in = -1;
	pipex->fd_out = -1;
	pipex->here_doc = false;
	pipex->is_urandom = false;
	pipex->is_invalid_input = false;
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
	return (*(int *)pipex_exit(pipex, NULL, END));
}
