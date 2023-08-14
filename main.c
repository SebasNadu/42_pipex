/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:05:13 by johnavar          #+#    #+#             */
/*   Updated: 2023/08/14 17:15:55 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

/* TO DO
 * init struct
 * checks args
 * parse cmds
 * parse args
 * exec
 * cleanup
*/

int	pipex(int f1, int f2, char **ag, char **envp)
{
	return (0);
}

int	main(int ac, char **ag, char **envp)
{
	int	f1;
	int	f2;

	f1 = open(ag[1], O_RDONLY);
	f2 = open(ag[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (f1 < 0 || f2 < 0)
		return (-1);
	pipex(f1, f2, ag, envp);
	return (0);
}
