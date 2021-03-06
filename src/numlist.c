/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numlist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 19:17:25 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/16 21:26:51 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignums.h"
#include <stdlib.h>

void				free_numlst(t_numlist *lst)
{
	t_numlist *tmp;

	while (lst != NULL)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
}

void				del_numlst(t_numlist **lst)
{
	free_numlst(*lst);
	*lst = NULL;
}

t_numlist			*new_numlst(unsigned long long num)
{
	t_numlist *lst;

	if ((lst = (t_numlist *)malloc(sizeof(t_numlist))) == NULL)
		return (NULL);
	lst->num = num;
	lst->next = NULL;
	lst->prev = NULL;
	return (lst);
}

unsigned long long	numlst_get_carry(t_numlist *cur)
{
	unsigned long long carry;

	carry = cur->num / BN_NUM_LEN_LIM;
	cur->num %= BN_NUM_LEN_LIM;
	return (carry);
}

unsigned long long	get_numlen(int len)
{
	unsigned long long numlen;

	numlen = 1L;
	while (--len > 0)
		numlen *= 10L;
	return (numlen);
}
