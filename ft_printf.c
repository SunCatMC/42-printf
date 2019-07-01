/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/26 01:05:00 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

static long long			get_s_num(t_popts *opts, va_list *argptr)
{
	if (opts->length == L_CHAR)
		return((signed char)va_arg(*argptr, int));
	if (opts->length == L_SHORT)
		return((signed short)va_arg(*argptr, int));
	if (opts->length == L_INT)
		return(va_arg(*argptr, int));
	if (opts->length == L_LONG)
		return(va_arg(*argptr, long));
	if (opts->length == L_LONGLONG)
		return(va_arg(*argptr, long long));
	return (0);
}

static unsigned long long	get_u_num(t_popts *opts, va_list *argptr)
{
	if (opts->length == L_CHAR)
		return((unsigned char)va_arg(*argptr, int));
	if (opts->length == L_SHORT)
		return((unsigned short)va_arg(*argptr, int));
	if (opts->length == L_LONG)
		return(va_arg(*argptr, unsigned long));
	if (opts->length == L_LONGLONG)
		return(va_arg(*argptr, unsigned long long));
	return(va_arg(*argptr, unsigned int));
}

static long double			get_ldbl(t_popts *opts, va_list *argptr)
{
	if (opts->length == L_LONGDOUBLE)
		return(va_arg(*argptr, long double));
	return ((long double)va_arg(*argptr, double));
}

static void					parse_conversion(const char **format, t_popts *opts,
				t_pbuff *pbuff, va_list *argptr)
{
	char				ch;
	unsigned int		base;

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
	else if (ch == 'f' || ch == 'F')
	{
		if (ch == 'F')
				opts->flags = opts->flags | P_LARGE;
		printf_f_ldbl(get_ldbl(opts, argptr), opts, pbuff);
	}
	else if (ch == 'b' || ch == 'u' || ch == 'o' || ch == 'x' || ch == 'X')
	{
		opts->flags = opts->flags | P_NUM;
		if (ch == 'u')
			base = 10;
		else if (ch == 'o')
			base = 8;
		else if (ch == 'b')
			base = 2;
		else
		{
			base = 16;
			if (ch == 'X')
				opts->flags = opts->flags | P_LARGE;
		}
		printf_int(get_u_num(opts, argptr), base, opts, pbuff);
	}
	++*format;
}

static void					printf_base(const char *format, t_pbuff *pbuff,
				va_list *argptr)
{
	char	*ptr;
	t_popts	opts;

	while ((ptr = ft_strchr(format, '%')))
	{
		putmem_pbuff(pbuff, format, ptr - format);
		format = ++ptr;
		parse_optionals(&format, &opts, argptr);
		parse_conversion(&format, &opts, pbuff, argptr);
	}
	putstr_pbuff(pbuff, format);
}

static void					init_pbuff(t_pbuff *pbuff)
{
	pbuff->size = 0;
	pbuff->printed = 0;
	pbuff->bigldbl.integ.least = NULL;
	pbuff->bigldbl.fract.least = NULL;
}

int							ft_printf(const char *format, ...)
{
	va_list	argptr;
	t_pbuff	pbuff;

	init_pbuff(&pbuff);
	va_start(argptr, format);
	printf_base(format, &pbuff, &argptr);
	va_end(argptr);
	flush_pbuff(&pbuff);
	free_numlst(pbuff.bigldbl.integ.least);
	free_numlst(pbuff.bigldbl.fract.least);
	return (pbuff.printed);
}
