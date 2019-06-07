/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/07 20:13:46 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

static void	printf_str(const char *str, t_popts *opts, t_pbuff *pbuff)
{
	const char	null[] = "(null)";
	char		*ptr;
	int			len;

	if (str == NULL)
		str = null;
	if (opts->flags & P_IS_PRECISE)
		ptr = (char *)ft_memchr(str, '\0', opts->precision);
	else if ((ptr = (char *)ft_memchr(str, '\0', opts->width)) == NULL)
		return (putstr_pbuff(pbuff, str));
	len = ptr ? ptr - str : opts->precision;
	if (!(opts->flags & F_LEFT) && len < opts->width)
		memset_pbuff(pbuff, ' ', opts->width - len);
	putmem_pbuff(pbuff, str, len);
	if ((opts->flags & F_LEFT) && len < opts->width)
		memset_pbuff(pbuff, ' ', opts->width - len);
}

static void	parse_conversion(const char **format, t_popts *opts,
						t_pbuff *pbuff, va_list *argptr)
{
	char	ch;

	if ((ch = **format) == '\0')
		return ;
	++*format;
	if (ch == 's')
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
