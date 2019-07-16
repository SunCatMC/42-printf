/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/16 21:26:30 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
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
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	return (length);
}

void		printf_f_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	int			length;
	t_bigldbl	*bigldbl;

	ldbl.num = num;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &(pbuff->bigldbl);
	bigldbl_round_up(bigldbl, -opts->precision);
	length = printf_f_int(&ldbl, opts, pbuff);
	printf_bigldbl_fract(opts, pbuff);
	printf_width_post(length, opts, pbuff);
}

