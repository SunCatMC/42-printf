/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/16 20:19:01 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/25 00:38:45 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include <stddef.h>

void		init_bigldbl_integ(t_ldbl *ldbl, t_bignum *bignum)
{
	unsigned long long	int_part;
	short				exp;

	exp = ldbl->bin.exp - EXP_BIAS;
	int_part = 0;
	if (!(ldbl->bin.exp == 0 || exp <= 0))
	{
		int_part = ldbl->bin.fract;
		if (exp < 64)
			int_part >>= (64 - exp);
	}
	init_bignum(bignum, int_part);
	if (exp > 64)
		bignum_mul_small(bignum, 2, exp - 64);
	if (bignum->least != NULL)
		mostnum_init_lens(bignum);
}

void		clean_up_fract(t_bignum *bignum)
{
	t_numlist *cur;

	if ((cur = bignum->least) == NULL)
		return ;
	while (cur->num == 0 && cur != bignum->most)
	{
		cur = cur->next;
		--bignum->count;
		--bignum->limit;
	}
	bignum->least = cur;
}

static void	multiply_fract(unsigned long long fract, t_bignum *bignum)
{
	bignum_mul_small(bignum, 5, exp < 0 ? 64 + -exp : 64);
	if (bignum->least == NULL)
		return ;
	bignum->limit = bignum->count;
	mostnum_init_lens(bignum);
	if (!(fract & FRACT_LAST_BIT) || exp < 0)
	{
		i = exp < 0 ? 64 + -exp : 64;
		bignum->limit = i / BN_MAX_DIGITS + 1;
		i %= BN_MAX_DIGITS;
		if (i == 0)
		{
			i = BN_MAX_DIGITS;
			--bignum->limit;
		}
		else if (bignum->limit > bignum->count)
			bignum_add_numlst(bignum, 0);
		bignum->most_num_len = 1;
		bignum->most_len = i;
		while (--i > 0)
			bignum->most_num_len *= 10;
	}
}

void		init_bigldbl_fract(t_ldbl *ldbl, t_bignum *bignum)
{
	unsigned long long	fract;
	short				exp;
	int					i;

	exp = ldbl->bin.exp - EXP_BIAS;
	if (ldbl->bin.fract == 0 || exp >= 64)
		init_bignum(bignum, 0);
	else
	{
		fract = ldbl->bin.fract;
		if (exp > 0)
			fract <<= exp;
		init_bignum(bignum, fract);
	}
	if (bignum->least == NULL || bignum_iszero(bignum))
	{
		bignum->limit = 1;
		return ;
	}
	multiply_fract(fract, bignum);
	if (bignum->least == NULL)
		return ;
	clean_up_fract(bignum);
}
