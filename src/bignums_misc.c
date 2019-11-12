#include "bignums.h"
#include <stdlib.h>

static int		find_cycle(t_bignum *bignum, t_numlist **result, int digit_exp)
{
	t_numlist	*cur;

	if (digit_exp >= 0)
	{
		cur = bignum->least;
		while (cur != bignum->most && digit_exp > BN_MAX_DIGITS)
		{
			digit_exp -= BN_MAX_DIGITS;
			cur = cur->next;
		}
	}
	else
	{
		cur = bignum->most;
		digit_exp += bignum->most_len;
		while (cur != bignum->least && digit_exp < 0)
		{
			digit_exp += BN_MAX_DIGITS;
			cur = cur->prev;
		}
	}
	*result = digit_exp >= 0 && digit_exp <= BN_MAX_DIGITS ? cur : NULL;
	return (digit_exp);
}

int				bignum_find_numlst(t_bignum *bignum, t_numlist **result,
															int digit_exp)
{
	int			i;

	if (bignum->limit > 0)
	{
		i = bignum->count;
		while (i++ < bignum->limit && digit_exp < 0)
			digit_exp += BN_MAX_DIGITS;
		if (digit_exp >= 0)
		{
			*result = NULL;
			return (0);
		}
	}
	return (find_cycle(bignum, result, digit_exp));
}

/*
** flag - cur != bignum->least
*/

int				check_rounding(unsigned long long num_high_sub,
			unsigned int digit, unsigned long long num_low_sub, int flag)
{
	if (digit > 5)
		return (1);
	else if (digit == 5)
	{
		if (num_low_sub != 0 || flag)
			return (1);
		else
			return (num_high_sub % 2);
	}
	return (0);
}

int				bignum_len(t_bignum *bignum)
{
	int		count;

	if (bignum->limit < 0)
		count = bignum->count;
	else
		count = bignum->limit;
	return (bignum->most_len + (count - 1) * BN_MAX_DIGITS);
}

int				bignum_iszero(t_bignum *bignum)
{
	return (bignum->most->num == 0 && bignum->most == bignum->least);
}
