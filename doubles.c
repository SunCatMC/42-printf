/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/24 18:19:00 by htryndam         ###   ########.fr       */
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
	memset_pbuff(pbuff, '0',
						printf_bignum(bignum, 0, max_printed_digits, pbuff));
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
	length = bignum_len(integ) + opts->precision
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
	t_popts opts_num;

	putchar_pbuff(pbuff, opts->flags & P_LARGE ? 'E' : 'e');
	opts_num.precision = -1;
	opts_num.width = 3;
	opts_num.flags = F_PLUS | F_ZERO;
	printf_s_int(exp, &opts_num, pbuff);
}

static int	get_exp(t_bigldbl *bigldbl)
{
	if (!bignum_iszero(&bigldbl->integ))
		return (bignum_len(&bigldbl->integ) - 1);
	else if (bignum_iszero(&bigldbl->fract))
		return (0);
	else
		return (bignum->count - bignum->limit
			- (bignum->most->num == 0 ? 1 : 0))
			* BN_MAX_DIGITS + bignum->saved_len - bignum->most_len - 1);
}

void		printf_e_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	int			length;
	t_bigldbl	*bigldbl;
	int			exp;
	t_bignum	*bignum;
	unsigned long long num_len;

	ldbl.num = num;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &(pbuff->bigldbl);
	bignum = bigldbl->integ.most->num == 0 ? &bigldbl->fract : &bigldbl->integ;
	num_len = bignum->saved_num_len;
	exp = get_exp(bigldbl);
	bigldbl_round_up(bigldbl, exp - opts->precision);
	if (bignum == &bigldbl->integ && num_len != bignum->saved_num_len)
	{
		num_len = bignum->saved_num_len;
		++exp;
	}
	length = 5 + (exp > 99 || exp < -99 ? 1 : 0) + opts->precision
					+ ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	put_special(length, ldbl.bin.sign, opts, pbuff);
	putchar_pbuff(pbuff, (bignum->most->num == 0
		&& bignum->most != bignum->least ? bignum->most->prev->num
		: bignum->most->num) / num_len + '0');
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	--bignum->saved_len;
	bignum->saved_num_len /= 10;
	if (bignum == &bigldbl->fract)
		memset_pbuff(pbuff, '0',
							printf_bignum(bignum, 1, opts->precision, pbuff));
	else
	{
		if (opts->precision > exp)
		{
			printf_bignum(bignum, 1, -1, pbuff);
			printf_bigldbl_fract(opts->precision - exp, pbuff);
		}
		else
			memset_pbuff(pbuff, '0',
							printf_bignum(bignum, 1, opts->precision, pbuff));
	}
	put_exp(exp, opts, pbuff);
	printf_width_post(length, opts, pbuff);
}

void		printf_g_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	int			length;
	t_bigldbl	*bigldbl;

	ldbl.num = num;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &(pbuff->bigldbl);
}
