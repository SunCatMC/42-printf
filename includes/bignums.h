/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignums.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/16 19:03:35 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/25 19:44:06 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIGNUMS_H
# define BIGNUMS_H
# include "shared.h"

# define EXP_MAX 32767
# define EXP_BIAS 16382
# define FRACT_LAST_BIT 0x8000000000000000
# define FRACT_INT_BIT FRACT_LAST_BIT

# define BN_MAX_DIGITS	12
# if BN_MAX_DIGITS < 10 || BN_MAX_DIGITS > 18
#  error BN_MAX_DIGITS must be between 10 and 18
# endif

/*
** number of digits in BN_NUM_MAX must be equal to BN_MAX_DIGITS
*/
# define BN_NUM_MAX		999999999999
# define BN_NUM_LEN_LIM	(BN_NUM_MAX + 1)
# define BN_NUM_LEN_MAX	(BN_NUM_LEN_LIM / 10)
# define BN_MUL_MAX		(0xFFFFFFFFFFFFFFFF / BN_NUM_LEN_LIM)

void				free_numlst(t_numlist *lst);
void				del_numlst(t_numlist **lst);
t_numlist			*new_numlst(unsigned long long num);
unsigned long long	numlst_get_carry(t_numlist *cur);
void				bignum_add_numlst(t_bignum *bignum, unsigned long long num);
void				mostnum_init_lens(t_bignum *bignum);
void				init_bignum(t_bignum *bignum, unsigned long long num);
int					bignum_find_numlst(t_bignum *bignum, t_numlist **result,
	int digit_exp);
unsigned int		bignum_round_up(t_bignum *bignum, int digit_exp);
unsigned int		bignum_inc_num(t_bignum *bignum, t_numlist *cur,
	unsigned long long num);
void				bignum_mul_small(t_bignum *bignum, unsigned int num,
	int count);
int					printf_bignum(t_bignum *bignum, int use_saved,
	int print_len, t_pbuff *pbuff);
void				printf_max_exp(t_ldbl *ldbl, t_popts *opts, t_pbuff *pbuff);
int					printf_init_ldbl(t_ldbl *ldbl, t_popts *opts,
	t_pbuff *pbuff);
void				init_bigldbl_integ(t_ldbl *ldbl, t_bignum *bignum);
void				init_bigldbl_fract(t_ldbl *ldbl, t_bignum *bignum);
void				bigldbl_round_up(t_bigldbl *bigldbl, int digit_exp);
int					check_rounding(unsigned long long num_high_sub,
	unsigned int digit, unsigned long long num_low_sub, int flag);
void				clean_up_fract(t_bignum *bignum);
int					bignum_len(t_bignum *bignum);
int					bignum_len_g(t_bignum *bignum, int precision);
int					bignum_iszero(t_bignum *bignum);
#endif
