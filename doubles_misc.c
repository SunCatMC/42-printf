/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles_misc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 22:51:34 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/13 18:36:45 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>

void		malloc_fail(t_bigldbl *bigldbl)
{
	free_numlst(bigldbl->integ.least);
	free_numlst(bigldbl->integ.most);
	exit(0);
}

void		printf_max_exp(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
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

void		init_bigldbl_integ(t_ldbl *ldbl, t_bigldbl *bigldbl)
{
	t_bignum			*bignum;
	unsigned long long	int_part;
	short				exp;

	exp = ldbl->bin.exp - EXP_BIAS;
	int_part = 0;
	bignum = &(bigldbl->integ);
	if (!(ldbl->bin.exp == 0 || exp <= 0))
	{
		int_part = ldbl->bin.fract;
		if (exp < 64)
			int_part >>= (64 - exp);
	}
	init_bignum(bignum, int_part);
	if (exp > 64)
		bignum_mul_small(bignum, 2, exp - 64);
	if (bignum->least == NULL)
		malloc_fail(bigldbl);
	mostnum_init_lens(bignum);
}

static void	clean_up_fract(t_bignum *bignum)
{
	t_numlist *cur;

	cur = bignum->least;
	while (cur->num == 0 && cur != bignum->most)
	{
		cur = cur->next;
		--bignum->count;
		--bignum->limit;
	}
	bignum->least = cur;
}

void		init_bigldbl_fract(t_ldbl *ldbl, t_bigldbl *bigldbl)
{
	t_bignum			*bignum;
	unsigned long long	fract;
	short				exp;
	int					i;

	exp = ldbl->bin.exp - EXP_BIAS;
	bignum = &(bigldbl->fract);
	if (ldbl->bin.fract == 0 || exp >= 64)
		init_bignum(bignum, 0);
	else
	{
		fract = ldbl->bin.fract;
		if (exp > 0)
			fract <<= exp;
		init_bignum(bignum, fract);
	}
	if (bignum->least == NULL)
		malloc_fail(bigldbl);
	bignum->most_num_len = 1;
	bignum->most_len = 1;
	if (bignum->least->num == 0 && bignum->least == bignum->most)
	{
		bignum->limit = 1;
		return ;
	}
	bignum_mul_small(bignum, 5, exp < 0 ? 64 + -exp : 64);
	if (bignum->least == NULL)
		malloc_fail(bigldbl);
	if (!(fract & FRACT_LAST_BIT) || exp < 0)
	{
		i = exp < 0 ? 64 + -exp : 64;
		bignum->limit = i / BN_MAX_DIGITS + 1;
		i %= BN_MAX_DIGITS;
		if (i != 0 && bignum->limit > bignum->count)
			bignum_add_numlst(bignum, 0);
		else if (i == 0)
		{
			i = BN_MAX_DIGITS;
			--bignum->limit;
		}
		while (--i > 0)
		{
			bignum->most_num_len *= 10;
			++bignum->most_len;
		}
	}
	else
	{
		bignum->limit = bignum->count;
		mostnum_init_lens(bignum);
	}
	if (bignum->least == NULL)
		malloc_fail(bigldbl);
	clean_up_fract(bignum);
}

void		bigldbl_round_up(t_bigldbl *bigldbl, int digit_exp)
{
	unsigned int	carry;
	int				i;

	carry = 0;
	if (digit_exp < 0)
	{
		i = bigldbl->fract.count;
		while (i++ < bigldbl->fract.limit && digit_exp < 0)
			digit_exp += BN_MAX_DIGITS;
		if (digit_exp >= 0)
			return ;
		carry = bignum_round_up(&(bigldbl->fract), digit_exp);
	}
	else if (digit_exp > 0)
		bignum_round_up(&(bigldbl->integ), digit_exp);
	else if (bigldbl->fract.limit == bigldbl->fract.count
	&& check_rounding(bigldbl->integ.least->num,
	bigldbl->fract.most->num / bigldbl->fract.most_num_len,
	bigldbl->fract.most->num, bigldbl->fract.most, &(bigldbl->fract)))
		bignum_inc_num(&(bigldbl->integ), bigldbl->integ.least, 1ul);
	if (carry != 0)
		bignum_inc_num(&(bigldbl->integ), bigldbl->integ.least, 1ul);
}
