/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/31 22:38:10 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include "ft_printf.h"

static void	put_special(int length, t_popts *opts, t_pbuff *pbuff)
{
	if (!(opts->flags & F_ZERO))
		printf_width_pre(length, opts, pbuff);
	printf_sign(opts, pbuff);
	if (opts->flags & F_ZERO)
		printf_width_pre(length, opts, pbuff);
}

static void	printf_bigldbl_fract(int max_printed_digits, t_pbuff *pbuff)
{
	t_bignum	*bignum;
	int			count;
	int			ret;

	bignum = &(pbuff->bigldbl.fract);
	count = bignum->limit;
	while (count-- > bignum->count && max_printed_digits >= BN_MAX_DIGITS)
	{
		memset_pbuff(pbuff, '0', BN_MAX_DIGITS);
		max_printed_digits -= BN_MAX_DIGITS;
	}
	if (count > bignum->count)
		return (memset_pbuff(pbuff, '0', max_printed_digits));
	if ((ret = printf_bignum(bignum, 0, max_printed_digits, pbuff)))
			memset_pbuff(pbuff, '0', ret);
}

static void printf_f_bigldbl(t_bigldbl *bigldbl, t_popts *opts, t_pbuff *pbuff)
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

void		printf_f_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	t_bigldbl	*bigldbl;

	ldbl.num = num;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &pbuff->bigldbl;
	bigldbl_round_up(bigldbl, -opts->precision);
	if (ldbl.bin.sign)
		opts->flags |= P_NEGATIVE;
	printf_f_bigldbl(bigldbl, opts, pbuff);
}

static void	put_exp(int exp, t_popts *opts, t_pbuff *pbuff)
{
	t_popts opts_num;

	putchar_pbuff(pbuff, opts->flags & P_LARGE ? 'E' : 'e');
	opts_num.precision = -1;
	opts_num.width = 3;
	opts_num.flags = F_PLUS | F_ZERO;
	printf_s_int(exp, &opts_num, pbuff);
}

static int	get_exp(t_bigldbl *bigldbl)
{
	t_bignum *bn;

	if (!bignum_iszero(&bigldbl->integ))
		return (bignum_len(&bigldbl->integ) - 1);
	else if (bignum_iszero(&bigldbl->fract))
		return (0);
	bn = &bigldbl->fract;
	return ((bn->count - bn->limit - (bn->most->num == 0 ? 1 : 0))
			* BN_MAX_DIGITS + bn->saved_len - bn->most_len - 1);
}

static void printf_e_bigldbl(t_bigldbl *bigldbl,
									int exp, t_popts *opts, t_pbuff *pbuff)
{
	int			length;
	t_bignum	*bignum;
	int			ret;

	length = 5 + (exp > 99 || exp < -99 ? 1 : 0) + opts->precision
					+ ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	put_special(length, opts, pbuff);
	bignum = bigldbl->integ.most->num == 0 ? &bigldbl->fract : &bigldbl->integ;
	putchar_pbuff(pbuff, (bignum->most->num == 0
		&& bignum->most != bignum->least ? bignum->most->prev->num
		: bignum->most->num) / bignum->saved_num_len + '0');
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	--bignum->saved_len;
	bignum->saved_num_len /= 10;
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

void		printf_e_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	t_bigldbl	*bigldbl;
	int			exp;
	int			tmp_len;

	ldbl.num = num;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &(pbuff->bigldbl);
	tmp_len = bigldbl->integ.saved_len;
	exp = get_exp(bigldbl);
	bigldbl_round_up(bigldbl, exp - opts->precision);
	if (bigldbl->integ.most->num != 0 && bigldbl->integ.saved_len != tmp_len)
		++exp;
	if (ldbl.bin.sign)
		opts->flags |= P_NEGATIVE;
	printf_e_bigldbl(bigldbl, exp, opts, pbuff);
}

static void printf_g_bigldbl(t_bigldbl *bigldbl, t_popts *opts, t_pbuff *pbuff)
{
	int			len_integ;
	int			len_fract;
	int			exp;

	exp = get_exp(bigldbl);
	len_integ = bignum_len_g(&bigldbl->integ, -1);
	if (opts->flags & F_SPECIAL)
		len_fract = opts->precision - len_integ;
	else if (exp >= -4 && exp < opts->precision)
		len_fract = bignum_len_g(&bigldbl->fract, opts->precision - len_integ);
	else
		len_fract = bignum_len_g(&bigldbl->fract, opts->precision - exp);
	if (exp >= -4 && exp < opts->precision)
	{
		opts->precision -= len_integ;
		if (opts->precision > len_fract)
			opts->precision = len_fract;
		return (printf_f_bigldbl(bigldbl, opts, pbuff));
	}
	--opts->precision;
	if (!(opts->flags & F_SPECIAL))
	{
		if (len_fract != 0)
			opts->precision = len_fract + exp - 1;
		else
		{
			len_integ = bignum_len_g(&bigldbl->integ, exp) - 1;
			if (len_integ < opts->precision)
				opts->precision = len_integ;
		}
	}
	printf_e_bigldbl(bigldbl, exp, opts, pbuff);
}

void		printf_g_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	t_bigldbl	*bigldbl;
	int			len_integ;
	int			len_fract;
	int			exp;

	ldbl.num = num;
	if (ldbl.bin.sign)
		opts->flags |= P_NEGATIVE;
	if (opts->precision == 0)
		opts->precision = 1;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &(pbuff->bigldbl);
	exp = get_exp(bigldbl);
	len_integ = bignum_len_g(&bigldbl->integ, -1);
	if (opts->flags & F_SPECIAL)
		len_fract = opts->precision - len_integ;
	else if (exp >= -4 && exp < opts->precision)
		len_fract = bignum_len_g(&bigldbl->fract, opts->precision - len_integ);
	else
		len_fract = bignum_len_g(&bigldbl->fract, opts->precision - exp);
	if (exp >= -4 && exp < opts->precision)
		bigldbl_round_up(bigldbl, -len_fract);
	else
		bigldbl_round_up(bigldbl, len_fract != 0 ? -len_fract + 1
							: len_integ - bignum_len_g(&bigldbl->integ, exp));
	printf_g_bigldbl(bigldbl, opts, pbuff);
}
