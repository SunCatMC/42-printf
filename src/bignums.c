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
	bignum->saved_num_len = bignum->most_num_len;
	bignum->saved_len = bignum->most_len;
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
	bignum->most_num_len = 1;
	bignum->most_len = 1;
	bignum->saved_num_len = 1;
	bignum->saved_len = 1;
}

int				bignum_find_numlst(t_bignum *bignum, t_numlist **result,
															int digit_exp)
{
	t_numlist	*cur;
	int			i;

	if (bignum->limit > 0)
	{
		i = bignum->count;
		while (i++ < bignum->limit && digit_exp < 0)
			digit_exp += BN_MAX_DIGITS;
		if (digit_exp >= 0)
		{
			*result = NULL;
			return (0);
		}
	}
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

/*
** flag - cur != bignum->least
*/
  
int				check_rounding(unsigned long long num_high_sub,
			unsigned int digit, unsigned long long num_low_sub, int flag)
{
	if (digit > 5)
		return (1);
	else if (digit == 5)
	{
		if (num_low_sub != 0 || flag)
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
							(num_len != 1 ? cur : cur->prev) != bignum->least))
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
			if ((bignum->limit < 0 || bignum->limit < bignum->count))
			{
				if ((carry = numlst_get_carry(cur)) != 0)
					bignum_add_numlst(bignum, carry);
				if (bignum->limit < 0)
					mostnum_init_lens(bignum);
				return (0);
			}
			carry = bignum->most->num / (bignum->most_num_len * 10);
			bignum->most->num %= bignum->most_num_len * 10;
			return (carry);
		}
		carry = numlst_get_carry(cur);
		cur = cur->next;
	}
	return (0);
}

void			bignum_mul_small(t_bignum *bignum, unsigned int num, int count)
{
	t_numlist			*cur;
	unsigned long long	carry;
	unsigned long long	mul;
	int					i;

	i = 0;
	while (count > 0)
	{
		if (i == 0 || i > count)
		{
			i = 0;
			mul = 1;
			while (count > 0 && mul * num < BN_MUL_MAX)
			{
				mul *= num;
				--count;
				i++;
			}
		}
		else
			count -= i;
		cur = bignum->least;
		carry = 0;
		while (1)
		{
			cur->num = cur->num * mul + carry;
			carry = numlst_get_carry(cur);
			if (cur == bignum->most)
				break ;
			cur = cur->next;
		}
		if (carry != 0)
			bignum_add_numlst(bignum, carry);
		if (bignum->least == NULL)
			return ;
	}
}

int			bignum_len(t_bignum *bignum)
{
	int		count;

	if (bignum->limit < 0)
		count = bignum->count;
	else
		count = bignum->limit;
	return (bignum->most_len + (count - 1) * BN_MAX_DIGITS);
}

int			bignum_len_g(t_bignum *bignum, int precision)
{
	int					len;
	int					sub_len;
	t_numlist			*cur;
	unsigned long long	num;

	if (bignum_iszero(bignum) || (precision <= 0 && bignum->limit >= 0))
		return (0);
	len = bignum_len(bignum);
	if (bignum->limit < 0 && precision < 0)
		return (len);
	if (len > precision)
		len = precision;
	if (len <= 0)
		return (0);
	sub_len = bignum->most_len - bignum->saved_len;
	if (bignum->most->num == 0)
		sub_len += BN_MAX_DIGITS;
	sub_len = bignum_find_numlst(bignum, &cur, -len - sub_len);
	if (cur == NULL)
		return (0);
	if (bignum->limit >= 0)
		sub_len = BN_MAX_DIGITS - sub_len;
	num = cur->num / get_numlen(sub_len);
	while (num == 0 && len > 0)
	{
		if (cur == bignum->most)
			return (0);
		cur = cur->next;
		if (cur == bignum->most && num == 0)
			return (0);
		num = cur->num;
		len -= sub_len;
		sub_len = BN_MAX_DIGITS;
	}
	while (len > 0 && num % 10 == 0)
	{
		--len;
		num /= 10;
	}
	return (len > 0 ? len : 0);
}

int			bignum_iszero(t_bignum *bignum)
{
	return (bignum->most->num == 0 && bignum->most == bignum->least);
}

int			printf_bignum(t_bignum *bignum, int use_saved,
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
