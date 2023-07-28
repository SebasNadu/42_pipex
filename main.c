/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:05:13 by johnavar          #+#    #+#             */
/*   Updated: 2023/07/28 14:30:15 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int	main(void)
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
}
