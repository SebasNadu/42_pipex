/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:05:13 by johnavar          #+#    #+#             */
/*   Updated: 2023/09/06 14:39:55 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

void	*pipex_child(t_pipex *pipex, int fd[2], char **envp, int i)
{
	close(fd[0]);
	if (i == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			return (pipex_exit(pipex, NULL, DUP_ERR));
		close(fd[1]);
	}
	else if (i == pipex->cmd_count - 1)
	{
		if (dup2(pipex->fd_out, STDOUT_FILENO) < 0)
			return (pipex_exit(pipex, NULL, DUP_ERR));
		close(pipex->fd_out);
	}
	else
	{
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			return (pipex_exit(pipex, NULL, DUP_ERR));
		if (dup2(fd[0], STDIN_FILENO) < 0)
			return (pipex_exit(pipex, NULL, DUP_ERR));
		close(fd[0]);
		close(fd[1]);
	}
	execve(pipex->cmd_paths[i], pipex->cmd_args[i], envp);
	return (pipex_exit(pipex, NULL, 2));
}

void	*pipex_process(t_pipex *pipex, char **envp)
{
	int		fd[2];
	pid_t	pid;
	int		i;

	i = -1;
	if (dup2(pipex->fd_in, STDIN_FILENO) < 0)
		return (pipex_exit(pipex, NULL, DUP_ERR));
	close(pipex->fd_in);
	while (++i < pipex->cmd_count)
	{
		if (pipe(fd) == -1)
			return (pipex_exit(pipex, NULL, PIPE_ERR));
		pid = fork();
		if (pid == -1)
			return (pipex_exit(pipex, NULL, FORK_ERR));
		if (pid == 0)
			pipex_child(pipex, fd, envp, i);
		close(fd[1]);
		if ((i < pipex->cmd_count) && (dup2(fd[0], STDIN_FILENO) == -1))
			return (pipex_exit(pipex, NULL, DUP_ERR));
		wait(NULL);
		close(fd[0]);
	}
	return (NULL);
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
	if (parse_cmd_paths(pipex, argc, argv, envp) == false)
		return (*(int *)pipex_exit(pipex, NULL, NO_PATH));
	if (parse_cmd_args(pipex, argc, argv) == false)
		return (*(int *)pipex_exit(pipex, NULL, NO_MEMORY));
	pipex_process(pipex, envp);
	return (*(int *)pipex_exit(pipex, NULL, END));
}
