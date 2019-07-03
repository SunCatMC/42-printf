/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles_misc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 22:51:34 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/03 23:27:24 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>

void	malloc_fail(t_bigldbl *bigldbl)
{
	free_numlst(bigldbl->integ.least);
	free_numlst(bigldbl->integ.most);
	exit(0);
}

void	printf_max_exp(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
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

void	init_bigldbl_integ(t_ldbl *ldbl, t_bigldbl *bigldbl)
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
	while (exp-- > 64)
	{
		bignum_mul_small(bignum, 2);
		if (bignum->least == NULL)
			malloc_fail(bigldbl);
	}
	mostnum_init_lens(bignum);
}

void	init_bigldbl_fract(t_ldbl *ldbl, t_bigldbl *bigldbl)
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
	if (bignum->least->num == 0 && bignum->least == bignum->most)
	{
		bignum->is_limited = B_IS_LIMITED;
		return ;
	}
	bignum->most_num_len = 1;
	bignum->most_len = 1;
	i = exp < 0 ? 64 + -exp : 64;
	while (i-- > 0)
		bignum_mul_small(bignum, 5);
	if (!(fract & FRACT_LAST_BIT) || exp < 0)
	{
		i = exp < 0 ? 64 + -exp : 64;
		bigldbl->saved_precision_count = i / BN_MAX_DIGITS + 1;
		if (bigldbl->saved_precision_count > bignum->count)
			bignum_add_numlst(bignum, 0);
		i %= BN_MAX_DIGITS;
		while (--i > 0)
		{
			bignum->most_num_len *= 10;
			++bignum->most_len;
		}
	}
	else
	{
		bigldbl->saved_precision_count = bignum->count;
		mostnum_init_lens(bignum);
	}
	if (bigldbl->saved_precision_count == bignum->count)
		bignum->is_limited = B_IS_LIMITED;
}

void	bigldbl_round_up(t_bigldbl *bigldbl, int digit_exp)
{
	unsigned int	carry;
	int				i;

	if (digit_exp < 0)
	{
		i = bigldbl->fract.count;
		while (i++ < bigldbl->saved_precision_count && digit_exp < 0)
			digit_exp += BN_MAX_DIGITS;
		if (digit_exp >= 0)
			return ;
		carry = bignum_round_up(&(bigldbl->fract), digit_exp);
		digit_exp = 0;
	}
	if (digit_exp >= 0 || carry > 0)
		if ((carry = bignum_round_up(&(bigldbl->integ), digit_exp)) != 0)
			bignum_add_numlst(&(bigldbl->integ), carry);
}
