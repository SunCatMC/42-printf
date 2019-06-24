/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignums.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 13:26:17 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/24 23:38:51 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
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
	if (bignum->most->next == NULL)
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

void		init_bignum(t_bignum *bignum, unsigned long long num)
{
	unsigned long long temp;

	temp = num >> 60;
	num &= ~(temp << 60);
	if (num > BN_NUM_MAX)
	{
		temp = (temp << 1) | (num == 1L << 59 ? 1 : 0);
		num &= ~(1L << 59);
	}
	bignum->count = 0;
	add_numlst(bignum, num);
	if (temp > 0)
		add_numlst(bignum, temp);
}

void		bignum_mul_digit(t_bignum *bignum, unsigned int num)
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

void		printf_bignum(t_bignum *bignum, t_pbuff *pbuff)
{
	t_numlist			*cur;
	unsigned long long	num;
	unsigned long long	num_len;

	cur = bignum->most;
	num_len = bignum->most_num_len;
	while (cur != NULL)
	{
		num = cur->num;
		while (num_len >= 10)
		{
			putchar_pbuff(pbuff, num / num_len + '0');
			num %= num_len;
			num_len /= 10;
		}
		putchar_pbuff(pbuff, num / num_len + '0');
		cur = cur->prev;
		num_len = BN_NUM_LEN_MAX;
	}
}
