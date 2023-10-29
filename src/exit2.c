/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:13:39 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/14 00:11:06 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	pipex_perror2(char *param, int err)
{
	if (err == NO_PATH)
		ft_putstr_fd("PATH variable is not set ", STDERR_FILENO);
	else if (err == NO_READ || err == NO_WRITE)
		ft_putstr_fd("permission denied: ", STDERR_FILENO);
	else if (err == FORK_ERR)
		ft_putstr_fd("Could not create the child process: ", STDERR_FILENO);
	else if (err == PIPE_ERR)
		ft_putstr_fd("Could not create the pipe: ", STDERR_FILENO);
	if (param && (err == NO_READ || err == NO_WRITE))
		ft_putstr_fd(param, STDERR_FILENO);
}

void	pipex_perror(char *param, int err)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	if (param && (err == CMD_NOT_FOUND || err == NO_FILE || err == NO_AUTH
			|| err == CMD_FAIL))
	{
		ft_putstr_fd("line 1: ", STDERR_FILENO);
		ft_putstr_fd(param, STDERR_FILENO);
	}
	if (err == CMD_NOT_FOUND)
		ft_putstr_fd(": command not found", STDERR_FILENO);
	else if (err == NO_FILE)
		ft_putstr_fd(": No such file or directory", STDERR_FILENO);
	else if (err == NO_AUTH)
		ft_putstr_fd("Permission denied: ", STDERR_FILENO);
	else if (err == CMD_FAIL)
		ft_putstr_fd(": command failed", STDERR_FILENO);
	else if (err == INV_ARGS)
		ft_putstr_fd("Invalid arguments: ", STDERR_FILENO);
	else if (err == NO_MEMORY)
		ft_putstr_fd("No memory left on device: ", STDERR_FILENO);
	else if (err == DUP_ERR)
		ft_putstr_fd("Could not duplicate the fd: ", STDERR_FILENO);
	else
		pipex_perror2(param, err);
	ft_putstr_fd("\n", STDERR_FILENO);
}
