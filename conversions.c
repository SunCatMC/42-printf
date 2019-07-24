/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/16 21:55:46 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/25 00:38:42 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static long long	get_s_num(t_popts *opts, va_list *argptr)
{
	if (opts->length == L_CHAR)
		return ((signed char)va_arg(*argptr, int));
	if (opts->length == L_SHORT)
		return ((signed short)va_arg(*argptr, int));
	if (opts->length == L_INT)
		return (va_arg(*argptr, int));
	if (opts->length == L_LONG)
		return (va_arg(*argptr, long));
	if (opts->length == L_LONGLONG)
		return (va_arg(*argptr, long long));
	return (0);
}

static void			u_int_conversion(char ch, t_popts *opts, t_pbuff *pbuff,
				va_list *argptr)
{
	unsigned long long	num;
	unsigned int		base;

	if (opts->length == L_CHAR)
		num = (unsigned char)va_arg(*argptr, int);
	else if (opts->length == L_SHORT)
		num = (unsigned short)va_arg(*argptr, int);
	else if (opts->length == L_LONG)
		num = va_arg(*argptr, unsigned long);
	else if (opts->length == L_LONGLONG)
		num = va_arg(*argptr, unsigned long long);
	else
		num = va_arg(*argptr, unsigned int);
	opts->flags = opts->flags | P_NUM;
	if (ch == 'u')
		base = 10;
	else if (ch == 'o')
		base = 8;
	else if (ch == 'b')
		base = 2;
	else
		base = 16;
	if (ft_isupper(ch))
		opts->flags = opts->flags | P_LARGE;
	printf_int(num, base, opts, pbuff);
}

static void			ldbl_conversion(char ch, t_popts *opts, t_pbuff *pbuff,
				va_list *argptr)
{
	long double num;

	if (ft_isupper(ch))
		opts->flags = opts->flags | P_LARGE;
	if (opts->length == L_LONGDOUBLE)
		num = va_arg(*argptr, long double);
	else
		num = (long double)va_arg(*argptr, double);
	if (ch == 'f' || ch == 'F')
		printf_f_ldbl(num, opts, pbuff);
	else if (ch == 'e' || ch == 'E')
		printf_e_ldbl(num, opts, pbuff);
	else if (ch == 'g' || ch == 'G')
		printf_g_ldbl(num, opts, pbuff);
}

void				parse_conversion(const char **format, t_popts *opts,
				t_pbuff *pbuff, va_list *argptr)
{
	char				ch;

	if ((ch = **format) == '\0')
		return ;
	else if (ch == 'c')
		printf_char((char)va_arg(*argptr, int), opts, pbuff);
	else if (ch == '%')
		printf_char('%', opts, pbuff);
	else if (ch == 's')
		printf_str(va_arg(*argptr, char *), opts, pbuff);
	else if (ch == 'p')
		printf_ptr((unsigned long long)va_arg(*argptr, void *), opts, pbuff);
	else if (ch == 'i' || ch == 'd')
		printf_s_int(get_s_num(opts, argptr), opts, pbuff);
	else if (ft_strchr("fFeEgG", ch))
		ldbl_conversion(ch, opts, pbuff, argptr);
	else if (ft_strchr("buoxX", ch))
		u_int_conversion(ch, opts, pbuff, argptr);
	++*format;
}
