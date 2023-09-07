/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:05:13 by johnavar          #+#    #+#             */
/*   Updated: 2023/09/07 19:36:54 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;
	int		i;
	int		err;

	if (argc < 5)
		return (1);
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		return (*(int *)pipex_exit(NULL, NULL, NO_MEMORY));
	init_pipex(pipex);
	if (parse_args(argc, argv, pipex) == false)
		return (1);
	if (parse_cmd_paths(pipex, argc, argv, envp) == false)
		return (*(int *)pipex_exit(pipex, NULL, NO_PATH));
	if (parse_cmd_args(pipex, argc, argv) == false)
		return (*(int *)pipex_exit(pipex, NULL, NO_MEMORY));
	i = -1;
	while (++i < pipex->cmd_count)
	{
		err = exec_pipex(pipex, envp, i);
		if (err != NO_ERR)
			return (*(int *)pipex_exit(pipex, NULL, err));
		wait(NULL);
	}
	return (*(int *)pipex_exit(pipex, NULL, END));
}
