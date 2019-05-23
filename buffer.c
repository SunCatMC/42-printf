/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:29:23 by htryndam          #+#    #+#             */
/*   Updated: 2019/05/24 00:00:16 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include <unistd.h>

int		print_printf_buff(t_pbuff *pbuff)
{
	return (write(1, pbuff->buff, pbuff->size));
}

int		putstr_printf_buff(t_pbuff *pbuff, const char *str)
{
	int	ret;

	ret = 0;
	while (*str != '\0')
	{
		while (pbuff->size < PRINTF_BUFF_SIZE && *str != '\0')
		{
			pbuff->buff[pbuff->size] = *str;
			str++;
			pbuff->size++;
		}
		if (pbuff->size == PRINTF_BUFF_SIZE)
		{
			if ((ret = print_printf_buff(pbuff)) < 0)
				return (ret);
			pbuff->size = 0;
		}
	}
	return (ret);
}

int		putmem_printf_buff(t_pbuff *pbuff, const char *mem, int size)
{
	int	ret;

	ret = 0;
	while (size > 0)
	{
		while (pbuff->size < PRINTF_BUFF_SIZE && size > 0)
		{
			pbuff->buff[pbuff->size] = *mem;
			mem++;
			size--;
			pbuff->size++;
		}
		if (pbuff->size == PRINTF_BUFF_SIZE)
		{
			if ((ret = print_printf_buff(pbuff)) < 0)
				return (ret);
			pbuff->size = 0;
		}
	}
	return (ret);
}
