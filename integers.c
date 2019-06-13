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

static void		printf_digit(unsigned int digit, t_popts *opts, t_pbuff *pbuff)
{
	if (digit < 10)
		putchar_pbuff(pbuff, digit + '0');
	else if (opts->flags & P_LARGE_X)
		putchar_pbuff(pbuff, digit - 10 + 'A');
	else
		putchar_pbuff(pbuff, digit - 10 + 'a');
}

static void		zero_case(unsigned int base, t_popts *opts, t_pbuff *pbuff)
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

static t_pint	init_lens(unsigned long long num, unsigned int base,
		t_popts *opts)
{
	t_pint pint;

	pint.num_len = 1;
	pint.len = 1;
	while (num / pint.num_len >= base)
	{
		pint.num_len *= base;
		++pint.len;
	}
	pint.precision = pint.len;
	if (base == 8 && (opts->flags & F_SPECIAL) && num > 0)
		++pint.precision;
	if (opts->precision >= 0 && pint.precision < opts->precision)
		pint.precision = opts->precision;
	pint.length = pint.precision;
	if ((opts->flags & P_SIGNED)
			&& (opts->flags & (P_NEGATIVE | F_SPACE | F_PLUS)))
		++pint.length;
	if (base == 16 && (opts->flags & F_SPECIAL)
			&& (num > 0 || opts->flags & P_PTR))
		pint.length += 2;
	if (base == 2 && opts->flags & F_SPECIAL)
		++pint.length;
	return (pint);
}

static void		put_special(unsigned long long num, unsigned int base,
		t_popts *opts, t_pbuff *pbuff)
{
	if (opts->flags & P_SIGNED)
	{
		if (opts->flags & P_NEGATIVE)
			putchar_pbuff(pbuff, '-');
		else if (opts->flags & F_PLUS)
			putchar_pbuff(pbuff, '+');
		else if (opts->flags & F_SPACE)
			putchar_pbuff(pbuff, ' ');
	}
	if (base == 16 && (opts->flags & F_SPECIAL)
				&& (num > 0 || opts->flags & P_PTR))
		putmem_pbuff(pbuff, (opts->flags & P_LARGE_X) ? "0X" : "0x", 2);
	if (base == 2 && opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, 'b');
}

void			printf_int(unsigned long long num, unsigned int base,
		t_popts *opts, t_pbuff *pbuff)
{
	t_pint pint;

	if (opts->precision == 0 && num == 0)
		return (zero_case(base, opts, pbuff));
	pint = init_lens(num, base, opts);
	if (!(opts->flags & F_ZERO))
		printf_width_pre(pint.length, opts, pbuff);
	put_special(num, base, opts, pbuff);
	if (opts->flags & F_ZERO)
		printf_width_pre(pint.length, opts, pbuff);
	if (pint.len < pint.precision)
		memset_pbuff(pbuff, '0', pint.precision - pint.len);
	while (pint.num_len >= base)
	{
		printf_digit(num / pint.num_len, opts, pbuff);
		num %= pint.num_len;
		pint.num_len /= base;
	}
	printf_digit(num, opts, pbuff);
	printf_width_post(pint.length, opts, pbuff);
}
