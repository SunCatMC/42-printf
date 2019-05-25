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

int		print_pbuff(t_pbuff *pbuff)
{
	return (write(1, pbuff->buff, pbuff->size));
}

int		putchar_pbuff(t_pbuff *pbuff, const char ch)
{
	int ret;

	ret = 0;
	pbuff->buff[pbuff->size] = ch;
	pbuff->size++;
	if (pbuff->size == PRINTF_BUFF_SIZE)
	{
		ret = print_pbuff(pbuff);
		pbuff->size = 0;
	}
	return (ret);
}

int		putstr_pbuff(t_pbuff *pbuff, const char *str)
{
	int	ret;

	ret = 0;
	while (*str != '\0')
	{
		ret += putchar_pbuff(pbuff, *str);
		str++;
	}
	return (ret);
}

int		putmem_pbuff(t_pbuff *pbuff, const char *mem, int size)
{
	int	ret;

	ret = 0;
	while (size-- > 0)
	{
		ret += putchar_pbuff(pbuff, *mem);
		mem++;
	}
	return (ret);
}
