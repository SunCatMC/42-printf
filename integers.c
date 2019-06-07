/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 22:28:53 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/07 22:28:55 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		printf_int(unsigned long long num, int base,
		t_popts *opts, t_pbuff *pbuff)
{
	unsigned long long	num_len;
	int					length;

	num_len = 1;
	length = 1;
	while (num / num_len >= base)
	{
		num_len *= base;
		++length;
	}
	if (opts->flags & (P_NEGATIVE | F_SPACE | F_PLUS))
		++length;
	if ((opts->flags & F_SPECIAL) && base != 10)
		length += base == 8 ? 1 : 2;
}

void		printf_s_int(signed long long num, int base,
		t_popts *opts, t_pbuff *pbuff)
{
	if (num < 0)
	{
		opts->flags = opts->flags | P_NEGATIVE;
		printf_int(-num, base, opts, pbuff);
	}
	else
		printf_int(num, base, opts, pbuff);
}
