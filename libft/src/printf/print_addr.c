/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_addr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 17:54:04 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/07/04 18:12:23 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	ft_print_addr(unsigned long int addr)
{
	int		count;
	char	*symbols;

	symbols = "0123456789abcdef";
	if (addr < 16)
		return (write(1, &symbols[addr], 1));
	else
	{
		count = ft_print_addr(addr / 16);
		return (count + ft_print_addr(addr % 16));
	}
}

static int	ft_print_p(unsigned long int p)
{
	int	count;

	count = 0;
	if (p == 0)
	{
		count += ft_print_s(NNULL);
		return (count);
	}
	count += ft_print_s("0x");
	count += ft_print_addr(p);
	return (count);
}

int	ft_print_ptr(unsigned long int ptr, t_print *flags)
{
	int	count;

	count = 0;
	if (ptr == 0)
		flags->width -= ft_strlen(NNULL) - 1;
	else
		flags->width -= 2;
	if (flags->left == 1)
		count += ft_print_p(ptr);
	if (count < flags->width)
		count += ft_print_pad(flags->width, ft_nbr_len(ptr, 16), 0);
	if (flags->left == 0)
		count += ft_print_p(ptr);
	return (count);
}
