/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 20:28:22 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/08 00:03:11 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

void	printf_char(char ch, t_popts *opts, t_pbuff *pbuff)
{
	printf_width_pre(1, opts, pbuff);
	putchar_pbuff(pbuff, ch);
	printf_width_post(1, opts, pbuff);
}

void	printf_str(const char *str, t_popts *opts, t_pbuff *pbuff)
{
	const char	null[] = "(null)";
	char		*ptr;
	int			len;

	if (str == NULL)
		str = null;
	if (opts->flags & P_PRECISE)
		ptr = (char *)ft_memchr(str, '\0', opts->precision);
	else if ((ptr = (char *)ft_memchr(str, '\0', opts->width)) == NULL)
		return (putstr_pbuff(pbuff, str));
	len = ptr ? ptr - str : opts->precision;
	printf_width_pre(len, opts, pbuff);
	putmem_pbuff(pbuff, str, len);
	printf_width_post(len, opts, pbuff);
}
