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

void	flush_pbuff(t_pbuff *pbuff)
{
	int ret;

	ret = write(1, pbuff->buff, pbuff->size);
	pbuff->printed = ret < 0 ? -1 : pbuff->printed + ret;
	pbuff->size = 0;
}

void	putchar_pbuff(t_pbuff *pbuff, const char ch)
{
	pbuff->buff[pbuff->size] = ch;
	pbuff->size++;
	if (pbuff->size == PRINTF_BUFF_SIZE)
		return (flush_pbuff(pbuff));
}

void	putstr_pbuff(t_pbuff *pbuff, const char *str)
{
	const char	*ptr;

	while ((ptr = ft_memccpy(&pbuff->buff[pbuff->size], str, '\0',
		PRINTF_BUFF_SIZE - pbuff->size)) == NULL)
	{
		str += PRINTF_BUFF_SIZE - pbuff->size;
		pbuff->size = PRINTF_BUFF_SIZE;
		flush_pbuff(pbuff);
	}
	ptr--;
	pbuff->size += ptr - &pbuff->buff[pbuff->size];
	// unreachable?
	//if (pbuff->size == PRINTF_BUFF_SIZE)
	//	return (flush_pbuff(pbuff));
}

void	putmem_pbuff(t_pbuff *pbuff, const char *mem, int size)
{
	int		len;

	while (size >= (len = PRINTF_BUFF_SIZE - pbuff->size))
	{
		ft_memcpy(&pbuff->buff[pbuff->size], mem, len);
		mem += len;
		size -= len;
		pbuff->size = PRINTF_BUFF_SIZE;
		flush_pbuff(pbuff);
	}
	ft_memcpy(&pbuff->buff[pbuff->size], mem, size);
	pbuff->size += size;
}
