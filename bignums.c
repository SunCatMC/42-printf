/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignums.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 13:26:17 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/26 22:36:06 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdlib.h>

void 		free_numlst(t_numlist *lst)
{
	t_numlist *tmp;

	while (lst != NULL)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
}

static void	malloc_fail(t_numlist *lst)
{
	free_numlst(lst);
	exit(0);
}

t_numlist	*new_numlst(unsigned long long num)
{
	t_numlist *lst;

	if ((lst = (t_numlist *)malloc(sizeof(t_numlist))) == NULL)
		return (NULL);
	lst->num = num;
	lst->next = NULL;
	lst->prev = NULL;
	return (lst);
}

void		add_numlst(t_bignum *bignum, unsigned long long num)
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
			malloc_fail(bignum->least);
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

void		mostnum_init_lens(t_bignum *bignum)
{
	bignum->most_num_len = 1;
	bignum->most_len = 1;
	while (bignum->most->num / bignum->most_num_len >= 10)
	{
		bignum->most_num_len *= 10;
		++bignum->most_len;
	}
}

static void	update_bignum_num(t_bignum *bignum, unsigned long long num)
{
	bignum->least->num += num % BN_NUM_LEN_LIM;
	if (bignum->least->num > BN_NUM_MAX || num > BN_NUM_MAX)
	{
		num /= BN_NUM_LEN_LIM;
		if (bignum->least->num > BN_NUM_MAX)
		{
			num += bignum->least->num / BN_NUM_LEN_LIM;
			bignum->least->num %= BN_NUM_LEN_LIM;
		}
		if (bignum->least != bignum->most)
			bignum->most->num += num;
		else
			add_numlst(bignum, num);
	}
}

void		init_bignum(t_bignum *bignum, unsigned long long num,
												unsigned long long round)
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
	add_numlst(bignum, num);
	if (temp > 0)
		add_numlst(bignum, temp);
	update_bignum_num(bignum, round);
}

void		bignum_mul_small(t_bignum *bignum, unsigned int num)
{
	t_numlist			*cur;
	unsigned long long	carry;

	cur = bignum->least;
	carry = 0;
	while (1)
	{
		cur->num = cur->num * num + carry;
		if (cur->num > BN_NUM_MAX)
		{
			carry = cur->num / BN_NUM_LEN_LIM;
			cur->num %= BN_NUM_LEN_LIM;
		}
		else
			carry = 0;
		if (cur == bignum->most)
			break ;
		cur = cur->next;
	}
	if (carry != 0)
		add_numlst(bignum, carry);
}

void		printf_bignum(t_bignum 	*bignum, t_pbuff *pbuff)
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
