/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnavar <johnavar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 16:04:52 by johnavar          #+#    #+#             */
/*   Updated: 2023/07/04 18:14:10 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "./libft.h"

# if defined (__linux__)
#  define NNULL "(nil)"
# elif defined (__APPLE__)
#  define NNULL "0x0"
# endif

typedef struct s_print
{
	int			spec;
	int			width;
	int			left;
	int			zero;
	int			precision;
	int			hash;
	int			space;
	int			plus;
	int			star;
}				t_print;

int		ft_printf(const char *strf, ...);
int		ft_parse_format(const char *format, va_list *ap);
int		ft_print_spec(char spec, va_list *ap, t_print *flags);
t_print	ft_initialize_tab(void);
int		ft_parse_flags(const char *format, int i, va_list *ap, t_print *flags);
int		ft_isflag(int c);
int		ft_istype(int c);
int		ft_isspec(int c);
void	ft_flag_left(t_print *flags);
void	ft_flag_star(va_list *ap, t_print *flags);
int		ft_flag_prec(const char *format, int i, va_list *ap, t_print *flags);
void	ft_flag_digit(char c, t_print *flags);
int		ft_print_c(char c);
int		ft_print_char(char c, t_print *flags);
int		ft_print_pad(int width, int size, int zero);
int		ft_print_string(char *str, t_print *flags);
int		ft_print_s(char *str);
int		ft_print_int(long n, char spec, t_print *flags);
int		ft_nbr_len(long int n, int base);
int		ft_print_digit(long n, int base, char mode);
int		ft_print_hex(long n, t_print *flags);
int		ft_print_ptr(unsigned long int ptr, t_print *flags);
int		ft_print_unsigned(long n, t_print *flags);

#endif
