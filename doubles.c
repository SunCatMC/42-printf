/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/26 22:36:12 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	printf_f_int(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
{
	int			length;
	t_bignum	*bignum;

	bignum = &(pbuff->bigdble.integ);
	length = bignum->most_len + (bignum->count - 1) * 60 + opts->precision + ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	if (!(opts->flags & F_ZERO))
		printf_width_pre(length, opts, pbuff);
	printf_sign(ldbl->bin.sign, opts, pbuff);
	if (opts->flags & F_ZERO)
		printf_width_pre(length, opts, pbuff);
	printf_bignum(bignum, pbuff);
	return (length);
}

static void	printf_f_fract(t_popts *opts, t_pbuff *pbuff)
{
	t_bignum	*bignum;
	int			count;

	bignum = &(pbuff->bigdble.fract);
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	bignum->max_digits = opts->precision;
	count = pbuff->bigdble.saved_precision_count;
	while (count-- > bignum->count && bignum->max_digits >= BN_MAX_DIGITS)
	{
		memset_pbuff(pbuff, '0', BN_MAX_DIGITS);
		bignum->max_digits -= BN_MAX_DIGITS;
	}
	if (count > bignum->count)
		return (memset_pbuff(pbuff, '0', bignum->max_digits));
	printf_bignum(bignum, pbuff);
}

void		printf_f_ldbl(long double num, t_popts *opts,
		t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	int			length;

	ldbl.num = num;
	if (ldbl.bin.exp == EXP_MAX)
		return (printf_max_exp(&ldbl, opts, pbuff));
	if (opts->precision < 0)
		opts->precision = 6;
	if (opts->width && (ldbl.bin.sign | (opts->flags & (F_SPACE | F_PLUS))))
		--opts->width;
	init_bignum_int(&ldbl, pbuff);
	init_bignum_fract(&ldbl, pbuff);
	length = printf_f_int(&ldbl, opts, pbuff);
	printf_f_fract(opts, pbuff);
	printf_width_post(length, opts, pbuff);
}
