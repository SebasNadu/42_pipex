/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:05:13 by johnavar          #+#    #+#             */
/*   Updated: 2023/08/04 11:31:23 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

/*int	main(void)
{
	pid_t	id;
	pid_t	id2;
	int		n;
	int		i;

	id = fork();
	if (id == 0)
	{
		printf("We are process y\n");
		n = 1;
	}
	else
	{
		id2 = fork();
		if (id2 == 0)
		{
			printf("We are process z\n");
			n = 6;
		}
		else
		{
			printf("We are parent process\n");
			n = 11;
			wait(NULL);
		}
		wait(NULL);
	}
	i = n;
	while (i < n + 5)
	{
		printf("%d,", i);
		fflush(stdout);
		i++;
	}
	if (id != 0)
		printf("\n");
	return (0);
}*/

int	main(int ac, char **av)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (2);
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("ping", "ping", "-c", "5", "google.com", NULL);
	}
	pid2 = fork();
	if (pid2 < 0)
		return (3);
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("grep", "grep", "rtt", NULL);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
