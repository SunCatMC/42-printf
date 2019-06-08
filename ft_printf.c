/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/08 17:32:14 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

static void	parse_conversion(const char **format, t_popts *opts,
						t_pbuff *pbuff, va_list *argptr)
{
	char			ch;
	unsigned int	base;

	if ((ch = **format) == '\0')
		return ;
	else if (ch == 'c' && ++*format)
		printf_char((char)va_arg(*argptr, int), opts, pbuff);
	else if (ch == '%' && ++*format)
		printf_char('%', opts, pbuff);
	else if (ch == 's' && ++*format)
		printf_str(va_arg(*argptr, char *), opts, pbuff);
	else if ((ch == 'i' || ch == 'd') && ++*format)
	{
		if (opts->length == L_CHAR)
			printf_s_int((signed char)va_arg(*argptr, int), opts, pbuff);
		else if (opts->length == L_SHORT)
			printf_s_int((signed short)va_arg(*argptr, int), opts, pbuff);
		else if (opts->length == L_INT)
			printf_s_int(va_arg(*argptr, int), opts, pbuff);
		else if (opts->length == L_LONG)
			printf_s_int(va_arg(*argptr, long), opts, pbuff);
		else if (opts->length == L_LONGLONG)
			printf_s_int(va_arg(*argptr, long long), opts, pbuff);
	}
	else if ((ch == 'u' || ch == 'o' || ch == 'x' || ch == 'X') && ++*format)
	{
		if (ch == 'u')
			base = 10;
		else if (ch == 'o')
			base = 8;
		else {
			base = 16;
			if (ch == 'X')
				opts->flags = opts->flags | P_LARGE_X;
		}
		if (opts->length == L_CHAR)
			printf_int((unsigned char)va_arg(*argptr, unsigned int), base, opts, pbuff);
		else if (opts->length == L_SHORT)
			printf_int((unsigned short)va_arg(*argptr, unsigned int), base, opts, pbuff);
		else if (opts->length == L_INT)
			printf_int(va_arg(*argptr, unsigned int), base, opts, pbuff);
		else if (opts->length == L_LONG)
			printf_int(va_arg(*argptr, unsigned long), base, opts, pbuff);
		else if (opts->length == L_LONGLONG)
			printf_int(va_arg(*argptr, unsigned long long), base, opts, pbuff);
	}
}

static void	printf_base(const char *format, t_pbuff *pbuff, va_list *argptr)
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

int			ft_printf(const char *format, ...)
{
	va_list	argptr;
	t_pbuff	pbuff;

	pbuff.size = 0;
	pbuff.printed = 0;
	va_start(argptr, format);
	printf_base(format, &pbuff, &argptr);
	va_end(argptr);
	flush_pbuff(&pbuff);
	return (pbuff.printed);
}
