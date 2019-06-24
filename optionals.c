/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optionals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 13:00:50 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/24 23:39:23 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

void		printf_width_pre(int len, t_popts *opts, t_pbuff *pbuff)
{
	if (!(opts->flags & F_LEFT) && len < opts->width)
		memset_pbuff(pbuff, opts->flags & F_ZERO && (!(opts->flags & P_NUM)
			|| opts->precision < 0) ? '0' : ' ', opts->width - len);
}

void		printf_width_post(int len, t_popts *opts, t_pbuff *pbuff)
{
	if ((opts->flags & F_LEFT) && len < opts->width)
		memset_pbuff(pbuff, ' ', opts->width - len);
}

void	printf_sign(int is_neg, t_popts *opts, t_pbuff *pbuff)
{
	if (is_neg)
		putchar_pbuff(pbuff, '-');
	else if (opts->flags & F_PLUS)
		putchar_pbuff(pbuff, '+');
	else if (opts->flags & F_SPACE)
		putchar_pbuff(pbuff, ' ');
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
	else if (ch == 'L')
		opts->length = L_LONGDOUBLE;
	else
		res = 0;
	return (res);
}

static int	parse_flags(char ch, t_popts *opts)
{
	int res;

	res = 1;
	if (ch == '#')
		opts->flags |= F_SPECIAL;
	else if (ch == '-')
		opts->flags |= F_LEFT;
	else if (ch == '+')
		opts->flags |= F_PLUS;
	else if (ch == ' ')
		opts->flags |= F_SPACE;
	else if (ch == '0')
		opts->flags |= F_ZERO;
	else
		res = 0;
	return (res);
}

void		parse_optionals(const char **fmt, t_popts *opts, va_list *argptr)
{
	opts->width = 0;
	opts->flags = 0;
	opts->precision = -1;
	opts->length = L_INT;
	while (1)
	{
		if (!parse_flags(**fmt, opts) && !parse_length(**fmt, opts))
		{
			if (ft_isdigit(**fmt))
				opts->width = simple_atoi_skip(fmt);
			else if (**fmt == '*')
			{
				opts->width = va_arg(*argptr, int);
				if (opts->width < 0)
				{
					opts->width = -opts->width;
					opts->flags |= F_LEFT;
				}
			}
			else if (**fmt == '.')
			{
				opts->precision = 0;
				++*fmt;
				if (ft_isdigit(**fmt))
					opts->precision = simple_atoi_skip(fmt);
				else if (**fmt == '*')
					opts->precision = va_arg(*argptr, int);
				else
					--*fmt;
			}
			else
				return ;
		}
		++*fmt;
	}
}
