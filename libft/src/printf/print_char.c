/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:06:33 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/07/04 18:12:33 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_c(char c)
{
	if (write(1, &c, 1) == -1)
		return (0);
	return (1);
}

int	ft_print_char(char c, t_print *flags)
{
	int	count;

	count = 0;
	if (flags->left == 1)
		count += ft_print_c(c);
	if (flags->width > 1)
		count += ft_print_pad(flags->width, 1, flags->zero);
	if (flags->left == 0)
		count += ft_print_c(c);
	return (count);
}
