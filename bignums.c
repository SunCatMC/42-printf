/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignums.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 13:26:17 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/03 23:27:09 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdlib.h>

void			bignum_add_numlst(t_bignum *bignum, unsigned long long num)
{
	if (bignum->least == NULL)
	{
		if ((bignum->least = new_numlst(num)) == NULL)
			exit(0);
		bignum->most = bignum->least;
	}
	else if (bignum->most == NULL)
	{
		bignum->least->num = num;
		bignum->most = bignum->least;
	}
	else if (bignum->most->next == NULL)
	{
		if ((bignum->most->next = new_numlst(num)) == NULL)
			return (del_numlst(&bignum->least));
		bignum->most->next->prev = bignum->most;
		bignum->most = bignum->most->next;
	}
	else
	{
		bignum->most = bignum->most->next;
		bignum->most->num = num;
	}
	++bignum->count;
}

void			mostnum_init_lens(t_bignum *bignum)
{
	bignum->most_num_len = 1;
	bignum->most_len = 1;
	while (bignum->most->num / bignum->most_num_len >= 10)
	{
		bignum->most_num_len *= 10;
		++bignum->most_len;
	}
}

void			init_bignum(t_bignum *bignum, unsigned long long num)
{
	unsigned long long temp;

	if (num > BN_NUM_MAX)
	{
		temp = num / BN_NUM_LEN_LIM;
		if (temp > 0)
			num %= BN_NUM_LEN_LIM;
	}
	else
		temp = 0;
	bignum->count = 0;
	bignum->max_digits = -1;
	bignum->most = NULL;
	bignum->is_limited = B_IS_UNLIMITED;
	bignum_add_numlst(bignum, num);
	if (temp > 0)
		bignum_add_numlst(bignum, temp);
}

int				bignum_find_numlst(t_bignum *bignum, t_numlist **result,
															int digit_exp)
{
	t_numlist	*cur;

	if (digit_exp >= 0)
	{
		cur = bignum->least;
		while (cur != NULL && digit_exp > BN_MAX_DIGITS)
		{
			digit_exp -= BN_MAX_DIGITS;
			cur = cur->next;
		}
	}
	else
	{
		cur = bignum->most;
		digit_exp += bignum->most_len;
		while (cur != NULL && digit_exp < 0)
		{
			digit_exp += BN_MAX_DIGITS;
			cur = cur->prev;
		}
	}
	*result = cur;
	return (digit_exp);
}

// what happens on border between numlists?

unsigned int	bignum_round_up(t_bignum *bignum, int digit_exp)
{
	t_numlist			*cur;
	unsigned long long	num_len;
	unsigned long long	num_high_sub;
	unsigned int		digit;

	digit_exp = bignum_find_numlst(bignum, &cur, digit_exp);
	if (cur == NULL)
		return (0);
	num_len = 1;
	while (--digit_exp > 0)
		num_len *= 10;
	num_high_sub = cur->num / num_len;
	digit = num_high_sub % 10;
	if (digit != 5)
		return (digit < 5 ? 0 : bignum_inc_num(bignum, cur, num_len * 10));
	if (cur != bignum->least || cur->num % num_len != 0)
		return (bignum_inc_num(bignum, cur, num_len * 10));
	return (num_high_sub / 10 % 2 == 0 ? 0 :
								bignum_inc_num(bignum, cur, num_len * 10));
}

unsigned int	bignum_inc_num(t_bignum *bignum, t_numlist *cur,
													unsigned long long num)
{
	unsigned long long	carry;
	unsigned long long	most_num_lim;

	carry = num;
	while (carry != 0)
	{
		cur->num += carry;
		if (cur == bignum->most)
		{
			if (!(bignum->is_limited))
				bignum_add_numlst(bignum, numlst_get_carry(cur));
			most_num_lim = bignum->most_num_len * 10;
			if (bignum->is_limited && bignum->most->num >= most_num_lim)
			{
				carry = bignum->most->num / bignum->most_num_len;
				bignum->most->num %= most_num_lim;
			}
			return (carry);
		}
		carry = numlst_get_carry(cur);
		cur = cur->next;
	}
	return (carry);
}

void			bignum_mul_small(t_bignum *bignum, unsigned int num)
{
	t_numlist			*cur;
	unsigned long long	carry;

	cur = bignum->least;
	carry = 0;
	while (1)
	{
		cur->num = cur->num * num + carry;
		carry = numlst_get_carry(cur);
		if (cur == bignum->most)
			break ;
		cur = cur->next;
	}
	if (carry != 0)
		bignum_add_numlst(bignum, carry);
}

void			printf_bignum(t_bignum 	*bignum, t_pbuff *pbuff)
{
	t_numlist			*cur;
	unsigned long long	num;
	unsigned long long	num_len;
	int					len;

	cur = bignum->most;
	num_len = bignum->most_num_len;
	len = 0;
	while (bignum->max_digits < 0 || len < bignum->max_digits)
	{
		num = cur->num;
		while (num_len >= 10 && (bignum->max_digits < 0
								|| len < bignum->max_digits))
		{
			putchar_pbuff(pbuff, num / num_len + '0');
			num %= num_len;
			num_len /= 10;
			++len;
		}
		if (bignum->max_digits < 0 || len < bignum->max_digits)
		{
			++len;
			putchar_pbuff(pbuff, num / num_len + '0');
		}
		if (cur == bignum->least)
			break ;
		cur = cur->prev;
		num_len = BN_NUM_LEN_MAX;
	}
	if (bignum->max_digits > 0 && len < bignum->max_digits)
		memset_pbuff(pbuff, '0', bignum->max_digits - len);
}
