/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doubles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 18:35:57 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/19 18:20:03 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

static void	printf_max_exp(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff)
{
	char	buff[5];
	char	*ptr;

	opts->precision = -1;
	opts->flags &= ~(F_ZERO);
	if (ldbl->bin.fract & ~(1L << 63))
		ft_memcpy(buff, opts->flags & P_LARGE ? "NAN" : "nan", 4);
	else
	{
		ptr = buff;
		if (ldbl->bin.sign)
			*ptr = '-';
		else if (opts->flags & F_PLUS)
			*ptr = '+';
		else if (opts->flags & F_SPACE)
			*ptr = ' ';
		else
			--ptr;
		++ptr;
		ft_memcpy(ptr, opts->flags & P_LARGE ? "INF" : "inf", 4);
	}
	printf_str(buff, opts, pbuff);
}

void		printf_f_ldbl(long double num, t_popts *opts,
		t_pbuff *pbuff)
{
	t_ldbl				ldbl;

	ldbl.num = num;
	if (ldbl.bin.exp == EXP_MAX)
		return (printf_max_exp(&ldbl, opts, pbuff));
	if (opts->precision < 0)
		opts->precision = 6;

}
