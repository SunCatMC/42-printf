/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/17 05:28:37 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include "ft_printf.h"

static void	put_special(int length, int sign, t_popts *opts, t_pbuff *pbuff)
{
	if (!(opts->flags & F_ZERO))
		printf_width_pre(length, opts, pbuff);
	printf_sign(sign, opts, pbuff);
	if (opts->flags & F_ZERO)
		printf_width_pre(length, opts, pbuff);
}

static void	printf_bigldbl_fract(int max_printed_digits, t_pbuff *pbuff)
{
	t_bignum	*bignum;
	int			count;

	bignum = &(pbuff->bigldbl.fract);
	count = bignum->limit;
	while (count-- > bignum->count && max_printed_digits >= BN_MAX_DIGITS)
	{
		memset_pbuff(pbuff, '0', BN_MAX_DIGITS);
		max_printed_digits -= BN_MAX_DIGITS;
	}
	if (count > bignum->count)
		return (memset_pbuff(pbuff, '0', max_printed_digits));
	printf_bignum(bignum, 0, max_printed_digits, pbuff);
}

void		printf_f_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	int			length;
	t_bigldbl	*bigldbl;
	t_bignum	*integ;

	ldbl.num = num;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &pbuff->bigldbl;
	bigldbl_round_up(bigldbl, -opts->precision);
	integ = &bigldbl->integ;
	length = integ->most_len + (integ->count - 1) * 60 + opts->precision
					+ ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	put_special(length, ldbl.bin.sign, opts, pbuff);
	printf_bignum(integ, 0, -1, pbuff);
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	printf_bigldbl_fract(opts->precision, pbuff);
	printf_width_post(length, opts, pbuff);
}

static void	put_exp(int exp, t_popts *opts, t_pbuff *pbuff)
{
	putchar_pbuff(pbuff, opts->flags & P_LARGE ? 'E' : 'e');
	opts->precision = 2;
	opts->width = 0;
	opts->flags = F_PLUS;
	printf_s_int(exp, opts, pbuff);
}

void		printf_e_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	int			length;
	t_bigldbl	*bigldbl;
	int			exp;
	t_bignum	*bignum;
	int			len;
	unsigned long long most_num_len;

	ldbl.num = num;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &(pbuff->bigldbl);
	length = 5 + opts->precision
					+ ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	put_special(length, ldbl.bin.sign, opts, pbuff);
	len = 0;
	bignum = bigldbl->integ.most->num == 0 ? &bigldbl->fract : &bigldbl->integ;
	most_num_len = bignum->most_num_len;
	if (bigldbl->integ.most->num == 0)
	{
		if (bigldbl->fract.most->num == 0)
			exp = 0;
		else
		{
			exp = -1 - (bigldbl->fract.limit - bigldbl->fract.count)
															* BN_MAX_DIGITS;
			while (most_num_len >= bigldbl->fract.most->num)
			{
				++len;
				most_num_len /= 10;
			}
			exp -= len;
		}
	}
	else
		exp = (bigldbl->integ.count - 1) * BN_MAX_DIGITS
				+ bigldbl->integ.most_len - 1;
	bigldbl_round_up(bigldbl, exp - opts->precision);
	putchar_pbuff(pbuff, bignum->most->num / most_num_len + '0');
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	if (bignum == &bigldbl->fract)
		printf_bignum(bignum, len + 1, opts->precision, pbuff);
	else
	{
		len = bignum->most_len - 1 + (bignum->count - 1) * BN_MAX_DIGITS;
		if (opts->precision > len)
		{
			printf_bignum(bignum, 1, -1, pbuff);
			printf_bigldbl_fract(opts->precision - len, pbuff);
		}
		else
			printf_bignum(bignum, 1, opts->precision, pbuff);
	}
	put_exp(exp, opts, pbuff);
	printf_width_post(length, opts, pbuff);
}
