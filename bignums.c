/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignums.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 13:26:17 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/11 19:16:07 by htryndam         ###   ########.fr       */
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
	bignum->most = NULL;
	bignum->limit = -1;
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
		while (cur != bignum->most && digit_exp > BN_MAX_DIGITS)
		{
			digit_exp -= BN_MAX_DIGITS;
			cur = cur->next;
		}
	}
	else
	{
		cur = bignum->most;
		digit_exp += bignum->most_len;
		while (cur != bignum->least && digit_exp < 0)
		{
			digit_exp += BN_MAX_DIGITS;
			cur = cur->prev;
		}
	}
	*result = digit_exp >= 0 && digit_exp <= BN_MAX_DIGITS ? cur : NULL;
	return (digit_exp);
}

int				check_rounding(unsigned long long num_high_sub,
	unsigned int digit, unsigned long long num_low_sub, t_numlist *cur,
	t_bignum *bignum)
{
	if (digit > 5)
		return (1);
	else if (digit == 5)
	{
		if (num_low_sub != 0 || cur != bignum->least)
			return (1);
		else
			return (num_high_sub % 2);
	}
	return (0);
}

unsigned int	bignum_round_up(t_bignum *bignum, int digit_exp)
{
	t_numlist			*cur;
	unsigned long long	num_len;
	unsigned long long	num_high_sub;
	unsigned long long	num_low_sub;
	unsigned int		digit;

	digit_exp = bignum_find_numlst(bignum, &cur, digit_exp);
	if (cur == NULL || (digit_exp == 0 && cur == bignum->least))
		return (0);
	num_len = 1;
	while (digit_exp-- > 0)
		num_len *= 10;
	num_high_sub = cur->num / num_len;
	num_low_sub = num_len != 1 ? cur->num % num_len : cur->prev->num;
	digit = num_len != 1 ? num_low_sub * 10 / num_len
											: num_low_sub / BN_NUM_LEN_MAX;
	num_low_sub %= num_len != 1 ? num_len / 10 : BN_NUM_LEN_MAX;
	if (check_rounding(num_high_sub, digit, num_low_sub,
										num_len != 1 ? cur : cur->prev, bignum))
		return (bignum_inc_num(bignum, cur, num_len));
	return (0);
}

unsigned int	bignum_inc_num(t_bignum *bignum, t_numlist *cur,
													unsigned long long num)
{
	unsigned long long	carry;

	carry = num;
	while (carry != 0)
	{
		cur->num += carry;
		if (cur == bignum->most)
		{
			carry = 0;
			if ((bignum->limit < 0 || bignum->limit < bignum->count)
				&& (carry = numlst_get_carry(cur)) != 0)
			{
				bignum_add_numlst(bignum, carry);
				return (0);
			}
			if (bignum->most->num >= bignum->most_num_len * 10)
			{
				carry = bignum->most->num / bignum->most_num_len;
				bignum->most->num %= bignum->most_num_len * 10;
			}
			return (carry);
		}
		carry = numlst_get_carry(cur);
		cur = cur->next;
	}
	return (0);
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

void			printf_bignum(t_bignum *bignum, int max_printed_digits,
																t_pbuff *pbuff)
{
	t_numlist			*cur;
	unsigned long long	num;
	unsigned long long	num_len;
	int					len;

	cur = bignum->most;
	num_len = bignum->most_num_len;
	len = 0;
	while (max_printed_digits < 0 || len < max_printed_digits)
	{
		num = cur->num;
		while (num_len >= 10 && (max_printed_digits < 0
								|| len < max_printed_digits))
		{
			putchar_pbuff(pbuff, num / num_len + '0');
			num %= num_len;
			num_len /= 10;
			++len;
		}
		if (max_printed_digits < 0 || len < max_printed_digits)
		{
			++len;
			putchar_pbuff(pbuff, num / num_len + '0');
		}
		if (cur == bignum->least)
			break ;
		cur = cur->prev;
		num_len = BN_NUM_LEN_MAX;
	}
	if (max_printed_digits > 0 && len < max_printed_digits)
		memset_pbuff(pbuff, '0', max_printed_digits - len);
}
