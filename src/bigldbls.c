/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bigldbls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 12:07:20 by htryndam          #+#    #+#             */
/*   Updated: 2019/11/29 12:07:21 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include "ft_printf.h"
#include <stdio.h>

void		printf_f_bigldbl(t_bigldbl *bigldbl, t_popts *opts, t_pbuff *pbuff)
{
	int			length;
	t_bignum	*integ;

	integ = &bigldbl->integ;
	length = bignum_len(integ) + opts->precision
					+ ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	put_special(length, opts, pbuff);
	printf_bignum(integ, 0, -1, pbuff);
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	printf_bigldbl_fract(opts->precision, pbuff);
	printf_width_post(length, opts, pbuff);
}

static int	printf_e_start(t_bignum *bignum, int exp, t_popts *opts,
															t_pbuff *pbuff)
{
	int length;

	length = 5 + (exp > 99 || exp < -99 ? 1 : 0) + opts->precision
					+ ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	put_special(length, opts, pbuff);
	putchar_pbuff(pbuff, (bignum->most->num == 0
		&& bignum->most != bignum->least ? bignum->most->prev->num
		: bignum->most->num) / bignum->saved_num_len + '0');
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	--bignum->saved_len;
	bignum->saved_num_len /= 10;
	return (length);
}

void		printf_e_bigldbl(t_bigldbl *bigldbl, int exp,
												t_popts *opts, t_pbuff *pbuff)
{
	int			length;
	t_bignum	*bignum;
	int			ret;

	bignum = bigldbl->integ.most->num == 0 ? &bigldbl->fract : &bigldbl->integ;
	length = printf_e_start(bignum, exp, opts, pbuff);
	if (bignum == &bigldbl->fract)
	{
		if ((ret = printf_bignum(bignum, 1, opts->precision, pbuff)))
			memset_pbuff(pbuff, '0', ret);
	}
	else
	{
		if (opts->precision > exp)
		{
			printf_bignum(bignum, 1, -1, pbuff);
			printf_bigldbl_fract(opts->precision - exp, pbuff);
		}
		else if ((ret = printf_bignum(bignum, 1, opts->precision, pbuff)))
			memset_pbuff(pbuff, '0', ret);
	}
	put_exp(exp, opts, pbuff);
	printf_width_post(length, opts, pbuff);
}

int			bigldbl_g_edge_exp(t_bigldbl *bigldbl, t_popts *opts)
{
	int			len_integ;
	int			len_fract;
	int			exp;
	t_bignum	*fract;
	t_bignum	*integ;

	exp = get_exp(bigldbl);
	fract = &bigldbl->fract;
	integ = &bigldbl->integ;
	len_integ = bignum_len_g(integ, -1);
	if (opts->flags & F_SPECIAL)
		len_fract = opts->precision - len_integ - 1
						- (exp >= -4 && exp < opts->precision ? 0 : exp);
	else if (exp >= -4 && exp < opts->precision)
		len_fract = bignum_len_g(fract, opts->precision
			- len_integ + (fract->most->num == 0 ? fract->most_len : 0));
	else
		len_fract = bignum_len_g(fract, opts->precision + -exp);
	if (exp >= -4 && exp < opts->precision)
		return (-len_fract - (fract->most_len - fract->saved_len));
	return (len_fract != 0 ? -len_fract
							: len_integ - bignum_len_g(integ, exp));
}

void		printf_g_bigldbl(t_bigldbl *bigldbl, t_popts *opts, t_pbuff *pbuff)
{
	int exp;
	int edge;

	exp = get_exp(bigldbl);
	edge = bigldbl_g_edge_exp(bigldbl, opts);
	if (exp >= -4 && exp < opts->precision)
	{
		if (edge >= 0 && edge < exp)
			opts->precision = edge;
		else if (exp > 0)
			opts->precision -= exp + 1;
		else
			opts->precision = -edge;
		return (printf_f_bigldbl(bigldbl, opts, pbuff));
	}
	if (exp - edge >= 0 && opts->precision > exp - edge)
		opts->precision = exp - edge;
	printf_e_bigldbl(bigldbl, exp, opts, pbuff);
}
