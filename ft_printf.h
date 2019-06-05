/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:12:03 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/04 18:10:42 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# define PRINTF_BUFF_SIZE 8
# include <stdarg.h>

typedef struct	s_pbuff	{
	char	buff[PRINTF_BUFF_SIZE];
	int		size;
	int		printed;
}				t_pbuff;
enum			e_printf_flags {
	F_SPECIAL = 1,
	F_LEFT = 2,
	F_PLUS = 4,
	F_BLANKS = 8,
	F_ZERO = 16,
	F_GROUPING = 32,
	L_CHAR = 64,
	L_SHORT = 128,
	L_LONG = 256,
	L_LONGLONG = 512,
	L_LONGDOUBLE = 1024,
	P_IS_PRECISE = 2048
};
typedef struct	s_popts {
	unsigned int	param;
	unsigned int	width;
	unsigned short	flags;
	unsigned int	precision;
}				t_popts;
void			flush_pbuff(t_pbuff *pbuff);
void			putstr_pbuff(t_pbuff *pbuff, const char *str);
void			putmem_pbuff(t_pbuff *pbuff, const char *mem, int size);
void			parse_optionals(const char **format, t_popts *opts,
		va_list *argptr);
#endif
