/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:24:42 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/23 13:29:03 by sebas_nadu       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	init_pipex(t_pipex *pipex)
{
	pipex->fd_in = -2;
	pipex->fd_out = -2;
	pipex->here_doc = false;
	pipex->is_urandom = false;
	pipex->is_in_cpy = false;
	pipex->cmd_paths = NULL;
	pipex->cmd_args = NULL;
	pipex->cmd_count = 0;
}

static int	fork_and_pipe(t_pipex *pipex, int fd[2], pid_t *pid, int i)
{
	int	dup_err;

	if (pipe(fd) == -1)
		return (PIPE_ERR);
	*pid = fork();
	if (*pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (FORK_ERR);
	}
	if (*pid == 0)
	{
		if (i == 0)
			dup_err = dup2(pipex->fd_in, STDIN_FILENO);
		if (i == pipex->cmd_count - 1)
			dup_err = dup2(pipex->fd_out, STDOUT_FILENO);
		else
			dup_err = dup2(fd[1], STDOUT_FILENO);
	}
	else
		dup_err = dup2(fd[0], STDIN_FILENO);
	if (dup_err == -1)
		return (DUP_ERR);
	return (NO_ERR);
}

/*broken_pipe_perror(pipex, i);*/
int	exec_pipex(t_pipex *pipex, char **envp, int i)
{
	int		fd[2];
	pid_t	pid;
	int		err;

	err = fork_and_pipe(pipex, fd, &pid, i);
	if (err != NO_ERR)
		return (err);
	if (pid == 0)
	{
		if (pipex->cmd_paths[i])
		{
			if (ft_strncmp(pipex->cmd_args[i][0], "exit", 4) == 0)
				pipex_exit(pipex, NULL, ft_atoi(pipex->cmd_args[i][1]));
			if (execve(pipex->cmd_paths[i], pipex->cmd_args[i], envp) != 0)
				pipex_exit(pipex, pipex->cmd_args[i][0], CMD_NOT_FOUND);
		}
		else
			pipex_exit(pipex, pipex->cmd_args[i][0], CMD_NOT_FOUND);
		// pipex_exit(pipex, NULL, 0);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
	return (NO_ERR);
}

void	pipex_controller(t_pipex *pipex, char **envp)
{
	int		i;
	int		err;
	int		status;

	i = -1;
	while (++i < pipex->cmd_count)
	{
		err = exec_pipex(pipex, envp, i);
		if (err != NO_ERR)
			pipex_exit(pipex, NULL, err);
	}
	i = -1;
	while (++i < pipex->cmd_count)
	{
		// waitpid(-1, &status, 0);
		wait(&status);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0 && i != 0)
		{
			printf("exit code %d\n", WEXITSTATUS(status));
			exit(WEXITSTATUS(status));
		}
			// pipex_exit(pipex, NULL, WEXITSTATUS(status));
	}
}
