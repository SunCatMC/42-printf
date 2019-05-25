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
		return (putmem_pbuff(pbuff, "(null)", 6));
	return (putstr_pbuff(pbuff, str));
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

static int	printf_base(const char *format, t_pbuff *pbuff, va_list *argptr)
{
	char	*ptr;
	int		size;
	int		ret;

	size = 0;
	while ((ptr = ft_strchr(format, '%')))
	{
		if ((ret = putmem_pbuff(pbuff, format, ptr - format)) < 0)
			return (-1);
		size += ret;
		format = ptr;
		size += parse_conversion(&format, pbuff, argptr);
	}
	ret = putstr_pbuff(pbuff, format);
	if (ret >= 0 && pbuff->size > 0)
		ret += print_pbuff(pbuff);
	return (ret >= 0 ? size + ret : -1);
}

int			ft_printf(const char *format, ...)
{
	int		ret;
	va_list	argptr;
	t_pbuff	pbuff;

	pbuff.size = 0;
	va_start(argptr, format);
	ret = printf_base(format, &pbuff, &argptr);
	va_end(argptr);
	return (ret);
}
