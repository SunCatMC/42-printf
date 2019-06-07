/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:12:03 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/07 18:51:10 by htryndam         ###   ########.fr       */
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
	L_CHAR = 32,
	L_SHORT = 64,
	L_LONG = 126,
	L_LONGLONG = 256,
	L_LONGDOUBLE = 512,
	P_IS_PRECISE = 1024
};
typedef struct	s_popts {
	int				param;
	int				width;
	unsigned short	flags;
	int				precision;
}				t_popts;
void			flush_pbuff(t_pbuff *pbuff);
void			putstr_pbuff(t_pbuff *pbuff, const char *str);
void			memset_pbuff(t_pbuff *pbuff, char ch, int size);
void			putmem_pbuff(t_pbuff *pbuff, const char *mem, int size);
void			parse_optionals(const char **format, t_popts *opts);
#endif
