/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 00:24:58 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/24 20:22:56 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	printf_width_pre(int len, t_popts *opts, t_pbuff *pbuff)
{
	if (!(opts->flags & F_LEFT) && len < opts->width)
		memset_pbuff(pbuff, opts->flags & F_ZERO && (!(opts->flags & P_NUM)
			|| opts->precision < 0) ? '0' : ' ', opts->width - len);
}

void	printf_width_post(int len, t_popts *opts, t_pbuff *pbuff)
{
	if ((opts->flags & F_LEFT) && len < opts->width)
		memset_pbuff(pbuff, ' ', opts->width - len);
}

void	printf_sign(t_popts *opts, t_pbuff *pbuff)
{
	if (opts->flags & P_NEGATIVE)
		putchar_pbuff(pbuff, '-');
	else if (opts->flags & F_PLUS)
		putchar_pbuff(pbuff, '+');
	else if (opts->flags & F_SPACE)
		putchar_pbuff(pbuff, ' ');
}
