/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numlist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 19:17:25 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/03 23:27:03 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdlib.h>

void 				free_numlst(t_numlist *lst)
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

	if (cur->num > BN_NUM_MAX)
	{
		carry = cur->num / BN_NUM_LEN_LIM;
		cur->num %= BN_NUM_LEN_LIM;
	}
	else
		carry = 0;
	return (carry);
}
