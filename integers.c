/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 22:28:53 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/09 00:30:09 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	printf_digit(unsigned int digit, t_popts *opts, t_pbuff *pbuff)
{
	if (digit < 10)
		putchar_pbuff(pbuff, digit + '0');
	else if (opts->flags & P_LARGE_X)
		putchar_pbuff(pbuff, digit - 10 + 'A');
	else
		putchar_pbuff(pbuff, digit - 10 + 'a');
}

static void zero_case(unsigned int base, t_popts *opts, t_pbuff *pbuff)
{
	int len;

	len = 0;
	if (opts->width != 0)
	{
		len = base == 8 && (opts->flags & F_SPECIAL) ? 0 : 1;
		printf_width_pre(len, opts, pbuff);
		putchar_pbuff(pbuff, base == 8 && (opts->flags & F_SPECIAL) ? 0 : ' ');
		printf_width_post(len, opts, pbuff);
	}
	else if (base == 8 && (opts->flags & F_SPECIAL))
		putchar_pbuff(pbuff, '0');
	else if (opts->flags & P_PTR)
		putmem_pbuff(pbuff, "0x", 2);
}

void		printf_int(unsigned long long num, unsigned int base,
		t_popts *opts, t_pbuff *pbuff)
{
	unsigned long long	num_len;
	int					precision;
	int					length;
	int					len;

	if (opts->precision == 0 && num == 0)
		return (zero_case(base, opts, pbuff));
	num_len = 1;
	len = 1;
	while (num / num_len >= base)
	{
		num_len *= base;
		++len;
	}
	precision = len;
	if (base == 8 && (opts->flags & F_SPECIAL) && num > 0)
		++precision;
	if (opts->precision >= 0 && precision < opts->precision)
		precision = opts->precision;
	length = precision;
	if ((opts->flags & P_SIGNED)
			&& (opts->flags & (P_NEGATIVE | F_SPACE | F_PLUS)))
		++length;
	if (base == 16 && (opts->flags & F_SPECIAL) && (num > 0 || opts->flags & P_PTR))
		length += 2;
	if (base == 2 && opts->flags & F_SPECIAL)
		++length;
	if (!(opts->flags & F_ZERO))
		printf_width_pre(length, opts, pbuff);
	if (opts->flags & P_SIGNED)
	{
		if (opts->flags & P_NEGATIVE)
			putchar_pbuff(pbuff, '-');
		else if (opts->flags & F_PLUS)
			putchar_pbuff(pbuff, '+');
		else if (opts->flags & F_SPACE)
			putchar_pbuff(pbuff, ' ');
	}
	if (base == 16 && (opts->flags & F_SPECIAL) && (num > 0 || opts->flags & P_PTR))
		putmem_pbuff(pbuff, (opts->flags & P_LARGE_X) ? "0X" : "0x", 2);
	if (base == 2 && opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, 'b');
	if (opts->flags & F_ZERO)
		printf_width_pre(length, opts, pbuff);
	if (len < precision)
		memset_pbuff(pbuff, '0', precision - len);
	while (num_len >= base)
	{
		printf_digit(num / num_len, opts, pbuff);
		num %= num_len;
		num_len /= base;
	}
	printf_digit(num, opts, pbuff);
	printf_width_post(length, opts, pbuff);
}


void		printf_s_int(signed long long num, t_popts *opts, t_pbuff *pbuff)
{
	opts->flags = opts->flags | P_SIGNED;
	if (num < 0)
	{
		opts->flags = opts->flags | P_NEGATIVE;
		printf_int(-num, 10, opts, pbuff);
	}
	else
		printf_int(num, 10, opts, pbuff);
}
