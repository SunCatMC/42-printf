/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 22:28:53 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/08 17:32:27 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
/*
static void	printf_digit()
{

}
*/
void		printf_int(unsigned long long num, unsigned int base,
		t_popts *opts, t_pbuff *pbuff)
{
	unsigned long long	num_len;
	int					precision;
	int					length;
	int					len;

	num_len = 1;
	len = 1;
	while (num / num_len >= base)
	{
		num_len *= base;
		++len;
	}
	precision = len;
	if (base == 8 && (opts->flags & F_SPECIAL))
		++precision;
	if ((opts->flags & P_PRECISE) && precision < opts->precision)
		precision = opts->precision;
	length = precision;
	if ((opts->flags & P_SIGNED)
			&& (opts->flags & (P_NEGATIVE | F_SPACE | F_PLUS)))
		++length;
	if (base == 16 && (opts->flags & F_SPECIAL))
		length += 2;
	if (opts->flags & P_SIGNED)
	{
		if (opts->flags & P_NEGATIVE)
			putchar_pbuff(pbuff, '-');
		else if (opts->flags & F_PLUS)
			putchar_pbuff(pbuff, '+');
		else if (opts->flags & F_SPACE)
			putchar_pbuff(pbuff, ' ');
	}
	printf_width_pre(length, opts, pbuff);
	if (base == 16 && (opts->flags & F_SPECIAL))
		putmem_pbuff(pbuff, (opts->flags & P_LARGE_X) ? "0X" : "0x", 2);
	if (len < precision)
		memset_pbuff(pbuff, '0', precision - len);
	while (num_len >= base)
	{
		putchar_pbuff(pbuff, num / num_len + '0');
		num %= num_len;
		num_len /= base;
	}
	putchar_pbuff(pbuff, num + '0');
	printf_width_post(length, opts, pbuff);
}


void		printf_s_int(signed long long num, t_popts *opts, t_pbuff *pbuff)
{
	opts->flags = opts->flags | P_SIGNED;
	if (num < 0)
	{
		opts->flags = opts->flags | P_NEGATIVE;
		printf_int(-num, 10, opts, pbuff);
	}
	else
		printf_int(num, 10, opts, pbuff);
}
