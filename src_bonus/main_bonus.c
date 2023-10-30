/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:05:13 by johnavar          #+#    #+#             */
/*   Updated: 2023/10/29 22:43:26 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;

	if (argc < 5)
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
