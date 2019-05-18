/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/05/16 23:44:52 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdarg.h>

static size_t	print_str(char *str)
{
	size_t len;

	if (str == NULL)
		return (write(1, "(null)", 6));
	len = ft_strlen(str);
	return (write(1, str, len));
}

static size_t	parse_conversion(char **format, va_list	*argptr)
{
	char	ch;

	++*format;
	ch = **format;
	if (ch == '\0')
		return (0);
	else
		++*format;
	if (ch == 's')
		return (print_str(va_arg(*argptr, char *)));
	return (0);
}

int				ft_printf(const char *format, ...)
{
	int		size;
	int		ret;
	char	*ptr;
	va_list	argptr;

	va_start(argptr, format);
	size = 0;
	while ((ptr = ft_strchr(format, '%')))
	{
		write(1, format, ptr - format);
		size += ptr - format;
		format = ptr;
		size += parse_conversion((char **)&format, &argptr);
	}
	va_end(argptr);
	ret = ft_strlen(format);
	write(1, format, ret);
	return (size + ret);
}
