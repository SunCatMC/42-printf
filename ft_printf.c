/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/07 22:24:10 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

static void	parse_conversion(const char **format, t_popts *opts,
						t_pbuff *pbuff, va_list *argptr)
{
	char	ch;

	if ((ch = **format) == '\0')
		return ;
	if (ch == 'c' && ++*format)
		printf_char((char)va_arg(*argptr, int), opts, pbuff);
	if (ch == '%' && ++*format)
		printf_char('%', opts, pbuff);
	if (ch == 's' && ++*format)
		printf_str(va_arg(*argptr, char *), opts, pbuff);
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
