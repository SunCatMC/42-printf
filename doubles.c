/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/26 01:33:10 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

unsigned long long	add_num_small(unsigned long long num, unsigned int digit)
{
	return (num + digit);
}

unsigned long long	mul_num_small(unsigned long long num, unsigned int digit)
{
	return (num * digit);
}

static void	printf_max_exp(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
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

static void	init_bignum_int(t_ldbl *ldbl, t_pbuff *pbuff)
{
	unsigned long long	int_part;
	t_bignum			*bignum;
	short				exp;

	exp = ldbl->bin.exp - EXP_BIAS;
	if (ldbl->bin.exp == 0 || exp <= 0)
		int_part = 0;
	else
		int_part = exp < 64 ? ldbl->bin.fract >> (64 - exp) : ldbl->bin.fract;
	bignum = &(pbuff->bignum);
	bignum->least = bignum->root;
	bignum->most = bignum->root;
	init_bignum(bignum, int_part);
	while (exp-- > 64)
		bignum_func(bignum, 2, &mul_num_small);
}

static void	init_bignum_fract(t_ldbl *ldbl, t_pbuff *pbuff)
{
	unsigned long long	fract;
	t_bignum			*bignum;
	short				exp;
	int					has_changed;
	int					i;

	exp = ldbl->bin.exp - EXP_BIAS;
	fract = exp < 64 ? ldbl->bin.fract << exp : ldbl->bin.fract;
	bignum = &(pbuff->bignum);
	if (bignum->most->next == NULL)
		add_numlst(bignum, 0);
	bignum->least = bignum->most;
	if (fract == 0)
		return ;
	//
	has_changed = 0;
	i = 0;
	while (exp++ < 0)
	{
		bignum_func(bignum, 10, &mul_num_small);
		if (i < 64)
		{
			if (fract & 1L << i)
				bignum_func(bignum, 5, &add_num_small);
			i++;
		}
	}
}

static int	printf_f_int(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
{
	int			length;
	t_bignum	*bignum;

	bignum = &(pbuff->bignum);
	mostnum_init_lens(bignum);
	length = bignum->most_len + (bignum->count - 1) * 60 + opts->precision + ((opts->precision || opts->flags & F_SPECIAL) ? 1 : 0);
	if (!(opts->flags & F_ZERO))
		printf_width_pre(length, opts, pbuff);
	printf_sign(ldbl->bin.sign, opts, pbuff);
	if (opts->flags & F_ZERO)
		printf_width_pre(length, opts, pbuff);
	printf_bignum(pbuff);
	return (length);
}

static void	print_f_fract(t_numlist	*tmp, t_popts *opts, t_pbuff *pbuff)
{
	t_bignum	*bignum;

	bignum = &(pbuff->bignum);
	if (opts->precision || opts->flags & F_SPECIAL)
		putchar_pbuff(pbuff, '.');
	bignum->least = bignum->most->next;
	bignum->most = tmp;
	bignum->max_digits = opts->precision;
	mostnum_init_lens(bignum);
	printf_bignum(pbuff);
}

void		printf_f_ldbl(long double num, t_popts *opts,
		t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	int			length;
	t_numlist	*tmp;

	ldbl.num = num;
	if (ldbl.bin.exp == EXP_MAX)
		return (printf_max_exp(&ldbl, opts, pbuff));
	if (opts->precision < 0)
		opts->precision = 6;
	if (opts->width && (ldbl.bin.sign | (opts->flags & (F_SPACE | F_PLUS))))
		--opts->width;
	init_bignum_int(&ldbl, pbuff);
	init_bignum_fract(&ldbl, pbuff);
	tmp = pbuff->bignum.most;
	pbuff->bignum.most = pbuff->bignum.least->prev;
	pbuff->bignum.least = pbuff->bignum.root;
	length = printf_f_int(&ldbl, opts, pbuff);
	print_f_fract(tmp, opts, pbuff);
	printf_width_post(length, opts, pbuff);
}
