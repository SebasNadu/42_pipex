/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:48:41 by johnavar          #+#    #+#             */
/*   Updated: 2023/07/04 18:11:46 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_isspec(int c)
{
	if (c == '-' || c == '0' || c == '.' || c == '*'
		|| c == '#' || c == ' ' || c == '+')
		return (1);
	return (0);
}

int	ft_istype(int c)
{
	if (c == 'c' || c == 's' || c == 'd' || c == 'i' || c == 'u'
		|| c == 'x' || c == 'X' || c == 'p' || c == '%')
		return (1);
	return (0);
}

int	ft_isflag(int c)
{
	if (ft_istype(c) || ft_isdigit(c) || ft_isspec(c))
		return (1);
	return (0);
}
