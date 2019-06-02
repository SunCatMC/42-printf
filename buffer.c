/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:29:23 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/02 23:01:42 by htryndam         ###   ########.fr       */
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
	return (ret);
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
	const char	*ptr;
	int			ret;

	while ((ptr = ft_memccpy(&pbuff->buff[pbuff->size], str, '\0',
		PRINTF_BUFF_SIZE - pbuff->size)) == NULL)
	{
		str += PRINTF_BUFF_SIZE - pbuff->size;
		pbuff->size = PRINTF_BUFF_SIZE;
		if ((ret = flush_pbuff(pbuff)) < 0)
			return (ret);
	}
	ptr--;
	pbuff->size += ptr - &pbuff->buff[pbuff->size];
	// unreachable?
	if (pbuff->size == PRINTF_BUFF_SIZE)
		return (flush_pbuff(pbuff));
	return (0);
}

int		putmem_pbuff(t_pbuff *pbuff, const char *mem, int size)
{
	int		len;
	int		ret;

	while (size >= (len = PRINTF_BUFF_SIZE - pbuff->size))
	{
		ft_memcpy(&pbuff->buff[pbuff->size], mem, len);
		mem += len;
		size -= len;
		pbuff->size = PRINTF_BUFF_SIZE;
		if ((ret = flush_pbuff(pbuff)) < 0)
			return (ret);
	}
	ft_memcpy(&pbuff->buff[pbuff->size], mem, size);
	pbuff->size += size;
	return (0);
}
