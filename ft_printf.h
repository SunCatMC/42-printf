/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:12:03 by htryndam          #+#    #+#             */
/*   Updated: 2019/05/24 00:00:09 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# define PRINTF_BUFF_SIZE 128

typedef struct	s_pbuff	{
	char	buff[PRINTF_BUFF_SIZE];
	int		size;
}				t_pbuff;
int		print_pbuff(t_pbuff *pbuff);
int		putstr_pbuff(t_pbuff *pbuff, const char *str);
int		putmem_pbuff(t_pbuff *pbuff, const char *mem, int size);
#endif
