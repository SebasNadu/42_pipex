/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 16:17:32 by johnavar          #+#    #+#             */
/*   Updated: 2023/07/04 18:13:42 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	ft_print_s_precision(const char *str, int precision)
{
	int	count;

	count = 0;
	while (str[count] && count < precision)
		count++;
	write(1, str, count);
	return (count);
}

static int	ft_print_str(const char *str, t_print *flags)
{
	int	count;

	count = 0;
	if (flags->precision >= 0)
	{
		count += ft_print_pad(flags->precision, ft_strlen(str), 0);
		count += ft_print_s_precision(str, flags->precision);
	}
	else
		count += ft_print_s_precision(str, ft_strlen(str));
	return (count);
}

#if defined(__linux__) || defined(__gnu_linux__)

int	ft_print_string(char *str, t_print *flags)
{
	int	count;

	count = 0;
	if (str == NULL && flags->precision >= 0 && flags->precision < 6)
	{
		count += ft_print_pad(flags->width, 0, 0);
		return (count);
	}
	if (str == NULL)
		str = "(null)";
	if (flags->precision >= 0 && (size_t)flags->precision > ft_strlen(str))
		flags->precision = ft_strlen(str);
	if (flags->left == 1)
		count += ft_print_str(str, flags);
	if (flags->precision >= 0)
		count += ft_print_pad(flags->width, flags->precision, 0);
	else
		count += ft_print_pad(flags->width, ft_strlen(str), 0);
	if (flags->left == 0)
		count += ft_print_str(str, flags);
	return (count);
}

#else

int	ft_print_string(char *str, t_print *flags)
{
	int	count;

	count = 0;
	if (str == NULL)
		str = "(null)";
	if (flags->precision >= 0 && (size_t)flags->precision > ft_strlen(str))
		flags->precision = ft_strlen(str);
	if (flags->left == 1)
		count += ft_print_str(str, flags);
	if (flags->precision >= 0)
		count += ft_print_pad(flags->width, flags->precision, 0);
	else
		count += ft_print_pad(flags->width, ft_strlen(str), 0);
	if (flags->left == 0)
		count += ft_print_str(str, flags);
	return (count);
}
#endif
