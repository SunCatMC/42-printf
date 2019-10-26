/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles_misc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 22:51:34 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/25 19:43:56 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>

static void	malloc_fail(t_bigldbl *bigldbl)
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

void		bigldbl_round_up(t_bigldbl *bigldbl, int digit_exp)
{
	unsigned int	carry;

	carry = 0;
	if (digit_exp < 0)
		carry = bignum_round_up(&(bigldbl->fract), digit_exp);
	else if (digit_exp > 0)
		bignum_round_up(&(bigldbl->integ), digit_exp);
	else if (bigldbl->fract.limit == bigldbl->fract.count
	&& check_rounding(bigldbl->integ.least->num,
	bigldbl->fract.most->num / bigldbl->fract.most_num_len,
	bigldbl->fract.most->num, bigldbl->fract.most, &(bigldbl->fract)))
		bignum_inc_num(&(bigldbl->integ), bigldbl->integ.least, 1ul);
	if (carry != 0)
		bignum_inc_num(&(bigldbl->integ), bigldbl->integ.least, 1ul);
	clean_up_fract(&bigldbl->fract);
}

int			printf_init_ldbl(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
{
	t_bigldbl *bigldbl;

	if (ldbl->bin.exp == EXP_MAX)
	{
		printf_max_exp(ldbl, opts, pbuff);
		return (0);
	}
	if (opts->precision < 0)
		opts->precision = 6;
	if (opts->width && (ldbl->bin.sign | (opts->flags & (F_SPACE | F_PLUS))))
		--opts->width;
	bigldbl = &(pbuff->bigldbl);
	init_bigldbl_integ(ldbl, &bigldbl->integ);
	if (bigldbl->integ.least == NULL)
		malloc_fail(bigldbl);
	init_bigldbl_fract(ldbl, &bigldbl->fract);
	if (bigldbl->fract.least == NULL)
		malloc_fail(bigldbl);
	return (1);
}
