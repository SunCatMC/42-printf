/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/08/01 12:58:26 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include "ft_printf.h"
#include <stdio.h>

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

void		printf_g_ldbl(long double num, t_popts *opts, t_pbuff *pbuff)
{
	t_ldbl		ldbl;
	t_bigldbl	*bigldbl;

	ldbl.num = num;
	if (ldbl.bin.sign)
		opts->flags |= P_NEGATIVE;
	if (opts->precision == 0)
		opts->precision = 1;
	if (!printf_init_ldbl(&ldbl, opts, pbuff))
		return ;
	bigldbl = &(pbuff->bigldbl);
	bigldbl_round_up(bigldbl, bigldbl_g_edge_exp(bigldbl, opts));
	printf_g_bigldbl(bigldbl, opts, pbuff);
}
