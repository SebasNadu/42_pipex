/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:21:30 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/29 22:39:39 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;

	if (argc != 5)
		return (1);
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		return (*(int *)pipex_exit(NULL, NULL, NO_MEMORY));
	init_pipex(pipex);
	if (parse_args(argc, argv, pipex) == false)
		return (*(int *)pipex_exit(pipex, NULL, INV_ARGS));
	if (parse_cmd_paths(pipex, argc, argv, envp) == false)
		return (*(int *)pipex_exit(pipex, NULL, NO_PATH));
	if (parse_cmd_args(pipex, argc, argv) == false)
		return (*(int *)pipex_exit(pipex, NULL, NO_MEMORY));
	pipex_controller(pipex, envp);
	return (*(int *)pipex_exit(pipex, NULL, END));
}
