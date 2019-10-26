/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 22:28:53 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/24 20:22:52 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		printf_digit(unsigned int digit, t_popts *opts, t_pbuff *pbuff)
{
	if (digit < 10)
		putchar_pbuff(pbuff, digit + '0');
	else if (opts->flags & P_LARGE)
		putchar_pbuff(pbuff, digit - 10 + 'A');
	else
		putchar_pbuff(pbuff, digit - 10 + 'a');
}

static t_pint	init_lens(unsigned long long num, unsigned int base,
		t_popts *opts)
{
	t_pint pint;

	pint.num_len = 1;
	pint.len = opts->precision == 0 && num == 0 ? 0 : 1;
	while (num / pint.num_len >= base)
	{
		pint.num_len *= base;
		++pint.len;
	}
	pint.precision = pint.len;
	if (base == 8 && (opts->flags & F_SPECIAL)
							&& (num > 0 || (num == 0 && opts->precision == 0)))
		++pint.precision;
	if (opts->precision >= 0 && pint.precision < opts->precision)
		pint.precision = opts->precision;
	pint.length = pint.precision;
	if ((opts->flags & P_SIGNED)
			&& (opts->flags & (P_NEGATIVE | F_SPACE | F_PLUS)))
		++pint.length;
	if ((base == 16 || base == 2) && (opts->flags & F_SPECIAL)
			&& (num > 0 || opts->flags & P_PTR))
		pint.length += 2;
	return (pint);
}

static void		put_special(unsigned long long num, unsigned int base,
		t_popts *opts, t_pbuff *pbuff)
{
	if (opts->flags & P_SIGNED)
		printf_sign(opts, pbuff);
	if (base == 16 && (opts->flags & F_SPECIAL)
				&& (num > 0 || opts->flags & P_PTR))
		putmem_pbuff(pbuff, (opts->flags & P_LARGE) ? "0X" : "0x", 2);
	if (base == 2 && opts->flags & F_SPECIAL)
		putmem_pbuff(pbuff, "0b", 2);
}

void			printf_int(unsigned long long num, unsigned int base,
		t_popts *opts, t_pbuff *pbuff)
{
	t_pint pint;

	pint = init_lens(num, base, opts);
	if (!(opts->flags & F_ZERO)
						|| (opts->flags & P_NUM && opts->precision >= 0))
		printf_width_pre(pint.length, opts, pbuff);
	put_special(num, base, opts, pbuff);
	if (opts->flags & F_ZERO && (!(opts->flags & P_NUM) || opts->precision < 0))
		printf_width_pre(pint.length, opts, pbuff);
	if (pint.len < pint.precision)
		memset_pbuff(pbuff, '0', pint.precision - pint.len);
	while (pint.num_len >= base)
	{
		printf_digit(num / pint.num_len, opts, pbuff);
		num %= pint.num_len;
		pint.num_len /= base;
	}
	if (opts->precision != 0 || num != 0)
		printf_digit(num, opts, pbuff);
	printf_width_post(pint.length, opts, pbuff);
}
