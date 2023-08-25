/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 16:01:25 by johnavar          #+#    #+#             */
/*   Updated: 2023/07/04 18:10:48 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_spec(char spec, va_list *ap, t_print *flags)
{
	int	count;

	count = 0;
	if (spec == '%')
		count += ft_print_char('%', flags);
	else if (spec == 'c')
		count += ft_print_char((char)va_arg(*ap, int), flags);
	else if (spec == 's')
		count += ft_print_string(va_arg(*ap, char *), flags);
	else if (spec == 'd' || spec == 'i')
		count += ft_print_int((long)(va_arg(*ap, int)), spec, flags);
	else if (spec == 'x' || spec == 'X')
		count += ft_print_hex((long)(va_arg(*ap, unsigned int)), flags);
	else if (spec == 'p')
		count += ft_print_ptr((unsigned long int)va_arg(*ap, void *), flags);
	else if (spec == 'u')
		count += ft_print_unsigned((long)(va_arg(*ap, unsigned int)), flags);
	return (count);
}

int	ft_parse_flags(const char *format, int i, va_list *ap, t_print *flags)
{
	while (format[++i] && ft_isflag(format[i]))
	{
		if (format[i] == '-')
			ft_flag_left(flags);
		if (format[i] == '#')
			flags->hash = 1;
		if (format[i] == ' ')
			flags->space = 1;
		if (format[i] == '+')
			flags->plus = 1;
		if (format[i] == '0' && flags->left == 0 && flags->width == 0)
			flags->zero = 1;
		if (format[i] == '.')
			i = ft_flag_prec(format, i, ap, flags);
		if (format[i] == '*')
			ft_flag_star(ap, flags);
		if (ft_isdigit(format[i]))
			ft_flag_digit(format[i], flags);
		if (ft_istype(format[i]))
		{
			flags->spec = format[i];
			break ;
		}
	}
	return (i);
}

#if defined(__linux__) || defined(__gnu_linux__)

int	ft_parse_format(const char *format, va_list *ap)
{
	int		i;
	int		x;
	int		count;
	t_print	flags;

	i = -1;
	count = 0;
	while (format[++i])
	{
		if (format[i] == '%' && format[i + 1] != '\0')
		{
			flags = ft_initialize_tab();
			x = ft_parse_flags(format, i, ap, &flags);
			if (flags.spec > 0)
				i = x;
			if (format[i] != '\0' && flags.spec > 0 && ft_istype(format[i]))
				count += ft_print_spec(format[i], ap, &flags);
			else if (format[i] != '\0')
				count += ft_print_c(format[i]);
		}
		else
			count += ft_print_c(format[i]);
	}
	return (count);
}

#else

int	ft_parse_format(const char *format, va_list *ap)
{
	int		i;
	int		count;
	t_print	flags;

	i = -1;
	count = 0;
	while (format[++i])
	{
		if (format[i] == '%' && format[i + 1] != '\0')
		{
			flags = ft_initialize_tab();
			i = ft_parse_flags(format, i, ap, &flags);
			if (format[i] != '\0' && flags.spec > 0 && ft_istype(format[i]))
				count += ft_print_spec(format[i], ap, &flags);
			else if (format[i] != '\0')
				count += ft_print_char(format[i], &flags);
		}
		else
			count += ft_print_c(format[i]);
	}
	return (count);
}

#endif

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		count;

	if (!format || *format == '\0')
		return (0);
	count = 0;
	va_start(ap, format);
	count = ft_parse_format(format, &ap);
	va_end(ap);
	return (count);
}
