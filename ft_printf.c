/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/04 18:10:56 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include <stdarg.h>

static void	print_str(char *str, t_pbuff *pbuff)
{
	if (str == NULL)
		return (putmem_pbuff(pbuff, "(null)", 6));
	putstr_pbuff(pbuff, str);
}

static void	parse_conversion(const char **format,
						t_pbuff *pbuff, va_list *argptr)
{
	char	ch;

	++*format;
	if ((ch = **format) == '\0')
		return ;
	else
		++*format;
	if (ch == 's')
		print_str(va_arg(*argptr, char *), pbuff);
}

static void	printf_base(const char *format, t_pbuff *pbuff, va_list *argptr)
{
	char	*ptr;

	while ((ptr = ft_strchr(format, '%')))
	{
		putmem_pbuff(pbuff, format, ptr - format);
		format = ptr;
		parse_conversion(&format, pbuff, argptr);
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
