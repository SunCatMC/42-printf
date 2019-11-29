/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignums_g.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 11:56:54 by htryndam          #+#    #+#             */
/*   Updated: 2019/11/29 11:56:58 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include <stdlib.h>

static int		bignum_g_cut_zeroes(t_bignum *bignum, int len)
{
	t_numlist			*cur;
	unsigned long long	num;
	int					tmp;

	tmp = bignum_find_numlst(bignum, &cur, -len);
	if (cur == NULL)
		return (0);
	num = cur->num / get_numlen(tmp);
	tmp = BN_MAX_DIGITS - tmp;
	while (num == 0 && len > 0 && cur != bignum->most)
	{
		cur = cur->next;
		num = cur->num;
		len -= tmp;
		tmp = BN_MAX_DIGITS;
	}
	while (len > 0 && num % 10 == 0)
	{
		--len;
		num /= 10;
	}
	return (len > 0 ? len : 0);
}

int				bignum_len_g(t_bignum *bignum, int precision)
{
	int			len;

	if (bignum_iszero(bignum) || (precision <= 0 && bignum->limit >= 0))
		return (0);
	len = bignum_len(bignum);
	if (precision < 0 && bignum->limit < 0)
		return (len);
	if (len > precision)
		len = precision;
	if (len <= 0)
		return (0);
	return (bignum_g_cut_zeroes(bignum, len));
}
