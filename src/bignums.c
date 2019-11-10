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

int				printf_bignum(t_bignum *bignum, int use_saved,
									int print_len, t_pbuff *pbuff)
{
	t_numlist			*cur;
	unsigned long long	num;
	unsigned long long	num_len;
	int					len;

	cur = bignum->most;
	num_len = use_saved ? bignum->saved_num_len : bignum->most_num_len;
	while (num_len == 0 || (use_saved && cur->num == 0))
	{
		if (cur == bignum->least)
			return (print_len > 0 ? print_len : 0);
		if (!(use_saved && cur->num == 0) && num_len == 0)
			num_len = BN_NUM_LEN_MAX;
		cur = cur->prev;
	}
	num = num_len < bignum->most_num_len || cur != bignum->most
									? cur->num % (num_len * 10) : cur->num;
	len = 0;
	while (print_len < 0 || len < print_len)
	{
		while (num_len >= 1 && (print_len < 0
								|| len < print_len))
		{
			++len;
			putchar_pbuff(pbuff, num / num_len + '0');
			num %= num_len;
			num_len /= 10;
		}
		if (cur == bignum->least)
			break ;
		cur = cur->prev;
		num_len = BN_NUM_LEN_MAX;
		num = cur->num;
	}
	return (print_len > 0 && len < print_len ? print_len - len : 0);
}
