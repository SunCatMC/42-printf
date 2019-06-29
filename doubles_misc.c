/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles_misc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 22:51:34 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/27 22:51:36 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void				printf_max_exp(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
{
	char	buff[5];
	char	*ptr;

	opts->precision = -1;
	opts->flags &= ~(F_ZERO);
	if (ldbl->bin.fract << 1)
		ft_memcpy(buff, opts->flags & P_LARGE ? "NAN" : "nan", 4);
	else
	{
		ptr = buff;
		if (ldbl->bin.sign)
		*ptr = '-';
		else if (opts->flags & F_PLUS)
		*ptr = '+';
		else if (opts->flags & F_SPACE)
		*ptr = ' ';
		else
			--ptr;
		++ptr;
		ft_memcpy(ptr, opts->flags & P_LARGE ? "INF" : "inf", 4);
	}
	printf_str(buff, opts, pbuff);
}

void				init_bignum_integ(t_ldbl *ldbl, t_bignum *bignum,
													unsigned long long round)
{
	unsigned long long	int_part;
	unsigned long long	int_round;
	short				exp;

	exp = ldbl->bin.exp - EXP_BIAS;
	int_part = 0;
	int_round = 0;
	if (!(ldbl->bin.exp == 0 || exp <= 0))
	{
		int_part = ldbl->bin.fract;
		int_round = 0;
		if (exp < 64)
		{
			int_part >>= (64 - exp);
			int_round = round >> (64 - exp);
		}
	}
	init_bignum(bignum, int_part, int_round);
	while (exp-- > 64)
		bignum_mul_small(bignum, 2);
	mostnum_init_lens(bignum);
}

void				init_bignum_fract(t_ldbl *ldbl, t_pbuff *pbuff,
													unsigned long long round)
{
	t_bignum			*bignum;
	unsigned long long	fract;
	short				exp;
	int					i;

	(void)round;
	exp = ldbl->bin.exp - EXP_BIAS;
	bignum = &(pbuff->bigldbl.fract);
	if (ldbl->bin.fract == 0 || exp >= 64)
		init_bignum(bignum, 0, 0);
	else
	{
		fract = ldbl->bin.fract;
		if (exp > 0)
		{
			fract <<= exp;
			round <<= exp;
		}
		init_bignum(bignum, fract, round);
	}
	if (bignum->least->num == 0 && bignum->least == bignum->most)
		return ;
	bignum->most_num_len = 1;
	bignum->most_len = 1;
	i = exp < 0 ? 64 + -exp : 64;
	while (i-- > 0)
		bignum_mul_small(bignum, 5);
	pbuff->bigldbl.saved_precision_count = -1;
	if (!(fract & FRACT_LAST_BIT) || exp < 0)
	{
		i = exp < 0 ? 64 + -exp : 64;
		pbuff->bigldbl.saved_precision_count = i / BN_MAX_DIGITS + 1;
		if (pbuff->bigldbl.saved_precision_count > bignum->count)
			add_numlst(bignum, 0);
		i %= BN_MAX_DIGITS;
		while (--i > 0)
		{
			bignum->most_num_len *= 10;
			++bignum->most_len;
		}
	}
	else
		mostnum_init_lens(bignum);
}

unsigned long long	round_up_bit(int shift, unsigned long long fract)
{
	int	is_odd;

	if (shift > 63 || shift < 0)
		return (0);
	fract <<= shift - 1;
	is_odd = (fract & FRACT_LAST_BIT) != 0;
	fract <<= 1;
	if (fract > FRACT_LAST_BIT || (fract == FRACT_LAST_BIT && is_odd))
	{
		return (FRACT_LAST_BIT >> shift);
	}
	return (0);
}
