/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:29:23 by htryndam          #+#    #+#             */
/*   Updated: 2019/05/25 20:55:53 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include <unistd.h>

int		print_pbuff(const t_pbuff *pbuff)
{
	return (write(1, pbuff->buff, pbuff->size));
}

int		flush_pbuff(t_pbuff *pbuff)
{
	int	ret;

	ret = print_pbuff(pbuff);
	if (ret > 0)
		pbuff->printed += ret;
	pbuff->size = 0;
	return (ret < 0 ? -1 : 0);
}

int		putchar_pbuff(t_pbuff *pbuff, const char ch)
{
	pbuff->buff[pbuff->size] = ch;
	pbuff->size++;
	if (pbuff->size == PRINTF_BUFF_SIZE)
		return (flush_pbuff(pbuff));
	return (0);
}

int		putstr_pbuff(t_pbuff *pbuff, const char *str)
{
	while (*str != '\0')
	{
		if (putchar_pbuff(pbuff, *str))
			return (-1);
		str++;
	}
	return (0);
}

int		putmem_pbuff(t_pbuff *pbuff, const char *mem, int size)
{
	while (size-- > 0)
	{
		if (putchar_pbuff(pbuff, *mem))
			return (-1);
		mem++;
	}
	return (0);
}
