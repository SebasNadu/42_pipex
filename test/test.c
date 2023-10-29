/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 20:23:42 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/16 22:23:17 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	main(int argc, char **argv)
{
	char	*str;
	char	*str2;
	int		i;

	i = 10;
	str = "Hello World!";
	str2 = argv[1];
	printf("%s: %d\n", str, i);
	printf("%s: %d\n", str2, argc);
	printf("%s: %d\n", argv[0], argc);
	printf("%s: %d\n", argv[2], argc);
	return (0);
}
