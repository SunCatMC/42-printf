/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:31:38 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/17 00:49:52 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static void	printf_base(const char *format, t_pbuff *pbuff,
				va_list *argptr)
{
	char	*ptr;
	t_popts	opts;

	while ((ptr = ft_strchrs(format, "%{")))
	{
		putmem_pbuff(pbuff, format, ptr - format);
		format = ptr + 1;
		if (*ptr == '%')
		{
			parse_optionals(&format, &opts, argptr);
			parse_conversion(&format, &opts, pbuff, argptr);
		}
		else if (*ptr == '{')
		{
			if (!parse_colors())
				putchar_pbuff(pbuff, '{');
		}
	}
	putstr_pbuff(pbuff, format);
}

static void	init_pbuff(t_pbuff *pbuff)
{
	pbuff->size = 0;
	pbuff->printed = 0;
	pbuff->bigldbl.integ.least = NULL;
	pbuff->bigldbl.fract.least = NULL;
}

int			ft_printf(const char *format, ...)
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
