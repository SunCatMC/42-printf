/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optionals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 13:00:50 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/26 01:05:25 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

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

static void	parse_precision(const char **fmt, t_popts *opts, va_list *argptr)
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

static int	parse_numbers(const char **fmt, t_popts *opts, va_list *argptr)
{
	int res;

	res = 1;
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
		parse_precision(fmt, opts, argptr);
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
	while (**fmt != '\0')
	{
		if (!parse_flags(**fmt, opts) && !parse_length(**fmt, opts))
		{
			if (!parse_numbers(fmt, opts, argptr))
				return ;
		}
		++*fmt;
	}
}
