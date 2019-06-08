/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optionals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 13:00:50 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/08 17:32:32 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

void		printf_width_pre(int len, t_popts *opts, t_pbuff *pbuff)
{
	if (!(opts->flags & F_LEFT) && len < opts->width)
		memset_pbuff(pbuff, ((opts->flags & F_ZERO)
			&& !(opts->flags & P_PRECISE)) ? '0' : ' ', opts->width - len);
}

void		printf_width_post(int len, t_popts *opts, t_pbuff *pbuff)
{
	if ((opts->flags & F_LEFT) && len < opts->width)
		memset_pbuff(pbuff, ' ', opts->width - len);
}

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

static int	parse_length(char ch, t_popts *opts)
{
	int res;

	res = 1;
	if (ch == 'h')
	{
		if (opts->length == L_SHORT)
			opts->length = L_CHAR;
		else if (opts->length != L_CHAR)
			opts->length = L_SHORT;
	}
	else if (ch == 'l')
	{
		if (opts->length == L_LONG)
			opts->length = L_LONGLONG;
		else if (opts->length != L_LONGLONG)
			opts->length = L_LONG;
	}
	else
		res = 0;
	return (res);
}

static int	parse_flags(char ch, t_popts *opts)
{
	int res;

	res = 1;
	if (ch == '#')
		opts->flags = opts->flags | F_SPECIAL;
	else if (ch == '-')
		opts->flags = opts->flags | F_LEFT;
	else if (ch == '+')
		opts->flags = opts->flags | F_PLUS;
	else if (ch == ' ')
		opts->flags = opts->flags | F_SPACE;
	else if (ch == '0')
		opts->flags = opts->flags | F_ZERO;
	else
		res = 0;
	return (res);
}

void		parse_optionals(const char **fmt, t_popts *opts, va_list *argptr)
{
	opts->width = 0;
	opts->flags = 0;
	opts->length = L_INT;
	while (1)
	{
		if (!parse_flags(**fmt, opts) && !parse_length(**fmt, opts))
		{
			if (ft_isdigit(**fmt))
				opts->width = simple_atoi_skip(fmt);
			else if (**fmt == '*')
				opts->width = va_arg(*argptr, int);
			else if (**fmt == '.')
			{
				++*fmt;
				if (ft_isdigit(**fmt))
					opts->precision = simple_atoi_skip(fmt);
				else if (**fmt == '*')
					opts->precision = va_arg(*argptr, int);
				opts->flags = opts->flags | P_PRECISE;
			}
			else
				break ;
		}
		++*fmt;
	}
}
