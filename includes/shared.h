/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/16 19:09:52 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/25 19:44:10 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
# define SHARED_H
# define PRINTF_BUFF_SIZE 4096

enum				e_printf_flags {
	F_SPECIAL = 1,
	F_LEFT = 2,
	F_PLUS = 4,
	F_SPACE = 8,
	F_ZERO = 16,
	P_NEGATIVE = 32,
	P_SIGNED = 64,
	P_LARGE = 128,
	P_NUM = 256,
	P_PTR = 512,
};
enum				e_printf_length {
	L_CHAR,
	L_SHORT,
	L_INT,
	L_LONG,
	L_LONGLONG,
	L_LONGDOUBLE,
};
typedef struct		s_num_bin {
	unsigned long long	fract:64;
	unsigned short		exp:15;
	unsigned short		sign:1;
}					t_num_bin;
typedef union		u_ldbl {
	long double		num;
	t_num_bin		bin;
}					t_ldbl;
typedef struct		s_numlist {
	unsigned long long	num;
	struct s_numlist	*next;
	struct s_numlist	*prev;
}					t_numlist;
typedef struct		s_bignum {
	t_numlist			*least;
	t_numlist			*most;
	unsigned long long	most_num_len;
	unsigned long long	saved_num_len;
	int					most_len;
	int					saved_len;
	int					count;
	int					limit;
}					t_bignum;
typedef struct		s_bigldbl {
	t_bignum	integ;
	t_bignum	fract;
}					t_bigldbl;
typedef struct		s_popts {
	int				param;
	int				width;
	unsigned short	flags;
	int				length;
	int				precision;
}					t_popts;
typedef struct		s_pint {
	int					len;
	unsigned long long	num_len;
	int					precision;
	int					length;
}					t_pint;
typedef struct		s_pbuff	{
	char		buff[PRINTF_BUFF_SIZE];
	int			size;
	int			printed;
	t_bigldbl	bigldbl;
}					t_pbuff;
void				flush_pbuff(t_pbuff *pbuff);
void				putchar_pbuff(t_pbuff *pbuff, char ch);
void				putstr_pbuff(t_pbuff *pbuff, const char *str);
void				memset_pbuff(t_pbuff *pbuff, char ch, int size);
void				putmem_pbuff(t_pbuff *pbuff, const char *mem, int size);
void				free_numlst(t_numlist *lst);
#endif
