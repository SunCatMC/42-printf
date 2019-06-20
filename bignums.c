/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignums.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 13:26:17 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/20 13:26:19 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

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

t_numlist	*insert_numlst(t_numlist *cur, t_numlist *next, t_numlist *prev)
{
	if (next != NULL)
		next->prev = cur;
	if (prev != NULL)
		prev->next = cur;
	cur->prev = prev;
	cur->next = next;
}

static void	malloc_fail(t_numlist *lst)
{
	free_numlst(lst);
	exit(0);
}

void		init_bignum(t_bignum *bignum, unsigned long long num)
{
	if (bignum->least == NULL)
	{
		if ((bignum->least = new_numlst(num)) == NULL)
			malloc_fail(NULL);
	}
	else
		bignum->least->num = num;
	bignum->most = bignum->least;
}

void		lshift_bignum(t_bignum *bignum)
{
	t_numlist *cur;

	cur = bignum->
	while ()
}
