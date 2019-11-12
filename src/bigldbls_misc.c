#include "bignums.h"
#include "ft_printf.h"
#include <stdio.h>

void	put_exp(int exp, t_popts *opts, t_pbuff *pbuff)
{
	t_popts opts_num;

	putchar_pbuff(pbuff, opts->flags & P_LARGE ? 'E' : 'e');
	opts_num.precision = -1;
	opts_num.width = 3;
	opts_num.flags = F_PLUS | F_ZERO;
	printf_s_int(exp, &opts_num, pbuff);
}

int		get_exp(t_bigldbl *bigldbl)
{
	t_bignum *bn;

	if (!bignum_iszero(&bigldbl->integ))
		return (bignum_len(&bigldbl->integ) - 1);
	else if (bignum_iszero(&bigldbl->fract))
		return (0);
	bn = &bigldbl->fract;
	return ((bn->count - bn->limit - (bn->most->num == 0 ? 1 : 0))
			* BN_MAX_DIGITS + bn->saved_len - bn->most_len - 1);
}

void	printf_bigldbl_fract(int max_printed_digits, t_pbuff *pbuff)
{
	t_bignum	*bignum;
	int			count;
	int			ret;

	bignum = &(pbuff->bigldbl.fract);
	count = bignum->limit;
	while (count-- > bignum->count && max_printed_digits >= BN_MAX_DIGITS)
	{
		memset_pbuff(pbuff, '0', BN_MAX_DIGITS);
		max_printed_digits -= BN_MAX_DIGITS;
	}
	if (count > bignum->count)
		return (memset_pbuff(pbuff, '0', max_printed_digits));
	if ((ret = printf_bignum(bignum, 0, max_printed_digits, pbuff)))
		memset_pbuff(pbuff, '0', ret);
}
