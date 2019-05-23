/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/05/24 00:00:05 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include <stdarg.h>

static int	print_str(char *str, t_pbuff *pbuff)
{
	if (str == NULL)
		return (putmem_printf_buff(pbuff, "(null)", 6));
	return (putstr_printf_buff(pbuff, str));
}

static int	parse_conversion(const char **format,
						t_pbuff *pbuff, va_list *argptr)
{
	char	ch;

	++*format;
	ch = **format;
	if (ch == '\0')
		return (0);
	else
		++*format;
	if (ch == 's')
		return (print_str(va_arg(*argptr, char *), pbuff));
	return (0);
}

int			ft_printf(const char *format, ...)
{
	int		size;
	int		ret;
	char	*ptr;
	va_list	argptr;
	t_pbuff	pbuff;

	pbuff.size = 0;
	va_start(argptr, format);
	size = 0;
	while ((ptr = ft_strchr(format, '%')))
	{
		if (putmem_printf_buff(&pbuff, format, ptr - format) < 0)
			return (-1);
		size += ptr - format;
		format = ptr;
		size += parse_conversion(&format, &pbuff, &argptr);
	}
	va_end(argptr);
	ret = putstr_printf_buff(&pbuff, format);
	if (ret >= 0 && pbuff.size > 0)
		ret += print_printf_buff(&pbuff);
	return (ret >= 0 ? size + ret : -1);
}
