
#include "bignums.h"
#include <stdlib.h>

void			bignum_add_numlst(t_bignum *bignum, unsigned long long num)
{
	if (bignum->least == NULL)
	{
		if ((bignum->least = new_numlst(num)) == NULL)
			exit(0);
		bignum->most = bignum->least;
	}
	else if (bignum->most == NULL)
	{
		bignum->least->num = num;
		bignum->most = bignum->least;
	}
	else if (bignum->most->next == NULL)
	{
		if ((bignum->most->next = new_numlst(num)) == NULL)
			return (del_numlst(&bignum->least));
		bignum->most->next->prev = bignum->most;
		bignum->most = bignum->most->next;
	}
	else
	{
		bignum->most = bignum->most->next;
		bignum->most->num = num;
	}
	++bignum->count;
}

void			mostnum_init_lens(t_bignum *bignum)
{
	bignum->most_num_len = 1;
	bignum->most_len = 1;
	while (bignum->most->num / bignum->most_num_len >= 10)
	{
		bignum->most_num_len *= 10;
		++bignum->most_len;
	}
	bignum->saved_num_len = bignum->most_num_len;
	bignum->saved_len = bignum->most_len;
}

void			init_bignum(t_bignum *bignum, unsigned long long num)
{
	unsigned long long temp;

	if (num > BN_NUM_MAX)
	{
		temp = num / BN_NUM_LEN_LIM;
		if (temp > 0)
			num %= BN_NUM_LEN_LIM;
	}
	else
		temp = 0;
	bignum->count = 0;
	bignum->most = NULL;
	bignum->limit = -1;
	bignum_add_numlst(bignum, num);
	if (temp > 0)
		bignum_add_numlst(bignum, temp);
	bignum->most_num_len = 1;
	bignum->most_len = 1;
	bignum->saved_num_len = 1;
	bignum->saved_len = 1;
}
