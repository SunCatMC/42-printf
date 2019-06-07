/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optionals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 13:00:50 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/07 18:51:16 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

static int	simple_atoi_skip(const char **str)
{
	int	num;

	num = 0;
	while (ft_isdigit(**str))
	{
		num = num * 10 + **str - '0';
		++*str;
	}
	--*str;
	return (num);
}

void		parse_optionals(const char **fmt, t_popts *opts)
{
	/*int		num;

	if (**fmt == '*')
	{
		num = va_arg(*argptr, int);
		opts->width = num < 0 ? -num : num;
	}
	*/
	opts->width = 0;
	opts->flags = 0;
	while (**fmt == '#' || **fmt == '-' || **fmt == '+' || **fmt == ' '
		|| **fmt == '0' || **fmt == '.' || ft_isdigit(**fmt))
	{
		if (ft_isdigit(**fmt))
			opts->width = simple_atoi_skip(fmt);
		else if (**fmt == '#')
			opts->flags = opts->flags | F_SPECIAL;
		else if (**fmt == '-')
			opts->flags = opts->flags | F_LEFT;
		else if (**fmt == '+')
			opts->flags = opts->flags | F_PLUS;
		else if (**fmt == ' ')
			opts->flags = opts->flags | F_BLANKS;
		else if (**fmt == '0')
			opts->flags = opts->flags | F_ZERO;
		else if (**fmt == '.')
		{
			++*fmt;
			opts->precision = simple_atoi_skip(fmt);
			opts->flags = opts->flags | P_IS_PRECISE;
		}
		++*fmt;
	}
}
