/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htryndam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 20:12:03 by htryndam          #+#    #+#             */
/*   Updated: 2019/07/24 20:22:49 by htryndam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "shared.h"
# include <stdarg.h>

void	parse_conversion(const char **format, t_popts *opts, t_pbuff *pbuff,
	va_list *argptr);
void	parse_optionals(const char **format, t_popts *opts, va_list *argptr);
void	printf_width_pre(int len, t_popts *opts, t_pbuff *pbuff);
void	printf_width_post(int len, t_popts *opts, t_pbuff *pbuff);
void	printf_sign(t_popts *opts, t_pbuff *pbuff);
void	printf_ptr(unsigned long long ptr, t_popts *opts, t_pbuff *pbuff);
void	printf_str(const char *str, t_popts *opts, t_pbuff *pbuff);
void	printf_char(char ch, t_popts *opts, t_pbuff *pbuff);
void	printf_int(unsigned long long num, unsigned int base, t_popts *opts,
	t_pbuff *pbuff);
void	printf_s_int(signed long long num, t_popts *opts, t_pbuff *pbuff);
void	printf_f_ldbl(long double num, t_popts *opts, t_pbuff *pbuff);
void	printf_e_ldbl(long double num, t_popts *opts, t_pbuff *pbuff);
void	printf_g_ldbl(long double num, t_popts *opts, t_pbuff *pbuff);
#endif
