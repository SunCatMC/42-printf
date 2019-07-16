/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/10 23:17:56 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	printf_f_int(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
{
	int			length;
	t_bignum	*bignum;

	bignum = &(pbuff->bigldbl.integ);
	length = bignum->most_len + (bignum->count - 1) * 60 + opts->precision
					+ ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	if (!(opts->flags & F_ZERO))
		printf_width_pre(length, opts, pbuff);
	printf_sign(ldbl->bin.sign, opts, pbuff);
	if (opts->flags & F_ZERO)
		printf_width_pre(length, opts, pbuff);
	printf_bignum(bignum, -1, pbuff);
	return (length);
}

static void	printf_f_fract(t_popts *opts, t_pbuff *pbuff)
{
	t_bignum	*bignum;
	int			count;
	int			max_printed_digits;

	bignum = &(pbuff->bigldbl.fract);
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	max_printed_digits = opts->precision;
	count = bignum->limit;
	while (count-- > bignum->count && max_printed_digits >= BN_MAX_DIGITS)
	{
		memset_pbuff(pbuff, '0', BN_MAX_DIGITS);
		max_printed_digits -= BN_MAX_DIGITS;
	}
	if (count > bignum->count)
		return (memset_pbuff(pbuff, '0', max_printed_digits));
	printf_bignum(bignum, max_printed_digits, pbuff);
}

void		printf_f_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	int			length;
	t_bigldbl	*bigldbl;

	ldbl.num = num;
	if (ldbl.bin.exp == EXP_MAX)
		return (printf_max_exp(&ldbl, opts, pbuff));
	if (opts->precision < 0)
		opts->precision = 6;
	if (opts->width && (ldbl.bin.sign | (opts->flags & (F_SPACE | F_PLUS))))
		--opts->width;
	bigldbl = &(pbuff->bigldbl);
	init_bigldbl_integ(&ldbl, bigldbl);
	init_bigldbl_fract(&ldbl, bigldbl);
	bigldbl_round_up(bigldbl, -opts->precision);
	length = printf_f_int(&ldbl, opts, pbuff);
	printf_f_fract(opts, pbuff);
	printf_width_post(length, opts, pbuff);
}
