/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:12:03 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/08 17:32:22 by htryndam         ###   ########.fr       */
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
	F_SPACE = 8,
	F_ZERO = 16,
	P_PRECISE = 32,
	P_NEGATIVE = 64,
	P_SIGNED = 128,
	P_LARGE_X = 256
};
enum			e_printf_length {
	L_CHAR,
	L_SHORT,
	L_INT,
	L_LONG,
	L_LONGLONG,
	L_LONGDOUBLE
};
typedef struct	s_popts {
	int				param;
	int				width;
	unsigned short	flags;
	int				length;
	int				precision;
}				t_popts;
void			flush_pbuff(t_pbuff *pbuff);
void			putchar_pbuff(t_pbuff *pbuff, char ch);
void			putstr_pbuff(t_pbuff *pbuff, const char *str);
void			memset_pbuff(t_pbuff *pbuff, char ch, int size);
void			putmem_pbuff(t_pbuff *pbuff, const char *mem, int size);
void			parse_optionals(const char **format, t_popts *opts,
		va_list *argptr);
void			printf_width_pre(int len, t_popts *opts, t_pbuff *pbuff);
void			printf_width_post(int len, t_popts *opts, t_pbuff *pbuff);

void			printf_str(const char *str, t_popts *opts, t_pbuff *pbuff);
void			printf_char(char ch, t_popts *opts, t_pbuff *pbuff);
void	 		printf_int(unsigned long long num,
		unsigned int base, t_popts *opts, t_pbuff *pbuff);
void			printf_s_int(signed long long num, t_popts *opts,
		t_pbuff *pbuff);
#endif
