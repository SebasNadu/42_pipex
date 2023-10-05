/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 11:47:55 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/07/04 18:13:02 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	ft_print_x_prefix(int spec)
{
	if (spec == 'X')
		ft_print_s("0X");
	else
		ft_print_s("0x");
	return (2);
}

static int	ft_print_x(long n, t_print *flags)
{
	int	count;

	count = 0;
	if (flags->zero == 0 && flags->hash == 1 && n != 0)
		count += ft_print_x_prefix(flags->spec);
	if (flags->precision >= 0)
		count += ft_print_pad(flags->precision, ft_nbr_len(n, 16), 1);
	count += ft_print_digit(n, 16, flags->spec);
	return (count);
}

static int	ft_print_x_width(long n, t_print *flags)
{
	int	count;

	count = 0;
	if (flags->precision >= 0 && flags->precision < ft_nbr_len(n, 16))
		flags->precision = ft_nbr_len(n, 16);
	if (flags->precision >= 0)
	{
		flags->width -= flags->precision;
		count += ft_print_pad(flags->width, 0, 0);
	}
	else
		count += ft_print_pad(flags->width,
				ft_nbr_len(n, 16) + (flags->hash * 2), flags->zero);
	return (count);
}

int	ft_print_hex(long n, t_print *flags)
{
	int	count;

	count = 0;
	if (flags->precision == 0 && n == 0)
	{
		count += ft_print_pad(flags->width, 0, flags->zero);
		return (count);
	}
	if (flags->zero == 1 && flags->hash == 1 && n != 0)
		count += ft_print_x_prefix(flags->spec);
	if (flags->left == 1)
		count += ft_print_x(n, flags);
	count += ft_print_x_width(n, flags);
	if (flags->left == 0)
		count += ft_print_x(n, flags);
	return (count);
}
