/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/19 18:20:03 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

static void	printf_max_exp(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
{
	char	buff[5];
	char	*ptr;

	opts->precision = -1;
	opts->flags &= ~(F_ZERO);
	if (ldbl->bin.fract & ~(1L << 63))
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

static void	printf_f_int(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
{
	unsigned long long	int_part;
	unsigned short		exp;

	(void)opts;
	(void)pbuff;
	exp = ldbl->bin.exp - EXP_BIAS;
	if (exp < 64)
		int_part = ldbl->bin.fract >> (64 - exp));
	else
		int_part = ldbl->bin.fract;
	init_bignum(&(pbuff.bignum), int_part);
	while (exp-- > 64)
		bignum_mul_digit(&(pbuff.bignum), 2);
}

void		printf_f_ldbl(long double num, t_popts *opts,
		t_pbuff *pbuff)
{
	t_ldbl	ldbl;

	ldbl.num = num;
	if (ldbl.bin.exp == EXP_MAX)
		return (printf_max_exp(&ldbl, opts, pbuff));
	if (opts->precision < 0)
		opts->precision = 6;
	if (ldbl.bin.exp > EXP_BIAS)
		printf_f_int(&ldbl, opts, pbuff);
}
