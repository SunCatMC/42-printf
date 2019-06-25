/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:12:03 by htryndam          #+#    #+#             */
/*   Updated: 2019/06/26 01:05:06 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# define PRINTF_BUFF_SIZE 4096
# include <stdarg.h>

enum			e_printf_flags {
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
typedef struct	s_pint {
	int					len;
	unsigned long long	num_len;
	int					precision;
	int					length;
}				t_pint;

typedef struct	s_num_bin {
	unsigned long long	fract:64;
	unsigned short		exp:15;
	unsigned char		sign:1;
}				t_num_bin;
typedef union	u_ldbl {
	long double		num;
	t_num_bin		bin;
}				t_ldbl;

# define EXP_MAX 32767
# define EXP_BIAS 16382
# define FRACT_INT_BIT 0x8000000000000000

typedef struct	s_numlist {
	unsigned long long	num;
	struct s_numlist	*next;
	struct s_numlist	*prev;
}				t_numlist;
typedef struct	s_bignum {
	t_numlist			*root;
	t_numlist			*least;
	t_numlist			*most;
	int					count;
	int					max_digits;
	int					most_len;
	unsigned long long	most_num_len;
}				t_bignum;

# define BN_NUM_MAX		999999999999999999
# define BN_NUM_LEN_LIM	1000000000000000000
# define BN_NUM_LEN_MAX	100000000000000000

typedef struct	s_pbuff	{
	char		buff[PRINTF_BUFF_SIZE];
	int			size;
	int			printed;
	t_bignum	bignum;
}				t_pbuff;

void			flush_pbuff(t_pbuff *pbuff);
void			putchar_pbuff(t_pbuff *pbuff, char ch);
void			putstr_pbuff(t_pbuff *pbuff, const char *str);
void			memset_pbuff(t_pbuff *pbuff, char ch, int size);
void			putmem_pbuff(t_pbuff *pbuff, const char *mem, int size);
void			parse_optionals(const char **format, t_popts *opts,
		va_list *argptr);
void			printf_width_pre(int len, t_popts *opts, t_pbuff *pbuff);
void			printf_width_post(int len, t_popts *opts, t_pbuff *pbuff);
void			printf_sign(int is_neg, t_popts *opts, t_pbuff *pbuff);
void			printf_ptr(unsigned long long ptr, t_popts *opts,
		t_pbuff *pbuff);
void			printf_str(const char *str, t_popts *opts, t_pbuff *pbuff);
void			printf_char(char ch, t_popts *opts, t_pbuff *pbuff);
void			printf_int(unsigned long long num, unsigned int base,
		t_popts *opts, t_pbuff *pbuff);
void			printf_s_int(signed long long num, t_popts *opts,
		t_pbuff *pbuff);
void			printf_f_ldbl(long double num, t_popts *opts, t_pbuff *pbuff);
void 			free_numlst(t_numlist *lst);
void			mostnum_init_lens(t_bignum *bignum);
void			init_bignum(t_bignum *bignum, unsigned long long num);
void			bignum_func(t_bignum *bignum, unsigned int num,
		unsigned long long (*f)(unsigned long long, unsigned int));
void			printf_bignum(t_pbuff *pbuff);
void		add_numlst(t_bignum *bignum, unsigned long long num);
#endif
