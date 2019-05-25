/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/05/25 20:56:02 by htryndam         ###   ########.fr       */
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

	while ((ptr = ft_strchr(format, '%')))
	{
		if ((putmem_pbuff(pbuff, format, ptr - format)) < 0)
			return (-1);
		format = ptr;
		if (parse_conversion(&format, pbuff, argptr) < 0)
			return (-1);
	}
	return (putstr_pbuff(pbuff, format));
}

int			ft_printf(const char *format, ...)
{
	int		ret;
	va_list	argptr;
	t_pbuff	pbuff;

	pbuff.size = 0;
	pbuff.printed = 0;
	va_start(argptr, format);
	ret = printf_base(format, &pbuff, &argptr);
	va_end(argptr);
	if (ret >= 0)
		ret = flush_pbuff(&pbuff);
	return (ret < 0 ? -1 : pbuff.printed);
}
