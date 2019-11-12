/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignums.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 13:26:17 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/31 22:38:07 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include <stdlib.h>

static unsigned long long	printf_bignum_init(t_bignum *bignum,
						t_numlist **cur, int use_saved, t_pint *pint)
{
	*cur = bignum->most;
	pint->num_len = use_saved ? bignum->saved_num_len : bignum->most_num_len;
	while (pint->num_len == 0 || (use_saved && (*cur)->num == 0))
	{
		if (*cur == bignum->least)
			return (0);
		if (!(use_saved && (*cur)->num == 0) && pint->num_len == 0)
			pint->num_len = BN_NUM_LEN_MAX;
		*cur = (*cur)->prev;
	}
	pint->len = 0;
	return (pint->num_len < bignum->most_num_len || (*cur) != bignum->most
						? (*cur)->num % (pint->num_len * 10) : (*cur)->num);
}

int			printf_bignum(t_bignum *bignum, int use_saved,
						int print_len, t_pbuff *pbuff)
{
	t_numlist			*cur;
	t_pint				pint;
	unsigned long long	num;

	pint.precision = print_len;
	num = printf_bignum_init(bignum, &cur, use_saved, &pint);
	if (cur == bignum->least && (pint.num_len == 0 || (use_saved
													&& cur->num == 0)))
		 return (print_len > 0 ? print_len : 0);
	while (print_len < 0 || pint.len < print_len)
	{
		while (pint.num_len >= 1 && (print_len < 0 || pint.len < print_len))
		{
			++pint.len;
			putchar_pbuff(pbuff, num / pint.num_len + '0');
			num %= pint.num_len;
			pint.num_len /= 10;
		}
		if (cur == bignum->least)
			break ;
		cur = cur->prev;
		pint.num_len = BN_NUM_LEN_MAX;
		num = cur->num;
	}
	return (print_len > 0 && pint.len < print_len ? print_len - pint.len : 0);
}
