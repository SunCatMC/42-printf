#include "bignums.h"
#include <stdlib.h>

unsigned int	bignum_round_up(t_bignum *bignum, int digit_exp)
{
	t_numlist			*cur;
	unsigned long long	num_len;
	unsigned long long	num_high_sub;
	unsigned long long	num_low_sub;
	unsigned int		digit;

	digit_exp = bignum_find_numlst(bignum, &cur, digit_exp);
	if (cur == NULL || (digit_exp == 0 && cur == bignum->least))
		return (0);
	num_len = 1;
	while (digit_exp-- > 0)
		num_len *= 10;
	num_high_sub = cur->num / num_len;
	num_low_sub = num_len != 1 ? cur->num % num_len : cur->prev->num;
	digit = num_len != 1 ? num_low_sub * 10 / num_len
											: num_low_sub / BN_NUM_LEN_MAX;
	num_low_sub %= num_len != 1 ? num_len / 10 : BN_NUM_LEN_MAX;
	if (check_rounding(num_high_sub, digit, num_low_sub,
							(num_len != 1 ? cur : cur->prev) != bignum->least))
		return (bignum_inc_num(bignum, cur, num_len));
	return (0);
}

unsigned int	bignum_inc_num(t_bignum *bignum, t_numlist *cur,
													unsigned long long num)
{
	unsigned long long	carry;

	carry = num;
	while (carry != 0)
	{
		cur->num += carry;
		if (cur == bignum->most)
		{
			if ((bignum->limit < 0 || bignum->limit < bignum->count))
			{
				if ((carry = numlst_get_carry(cur)) != 0)
					bignum_add_numlst(bignum, carry);
				if (bignum->limit < 0)
					mostnum_init_lens(bignum);
				return (0);
			}
			carry = bignum->most->num / (bignum->most_num_len * 10);
			bignum->most->num %= bignum->most_num_len * 10;
			return (carry);
		}
		carry = numlst_get_carry(cur);
		cur = cur->next;
	}
	return (0);
}

static void		mul_cycle(t_bignum *bignum, unsigned long long mul)
{
	unsigned long long	carry;
	t_numlist			*cur;

	cur = bignum->least;
	carry = 0;
	while (1)
	{
		cur->num = cur->num * mul + carry;
		carry = numlst_get_carry(cur);
		if (cur == bignum->most)
			break ;
		cur = cur->next;
	}
	if (carry != 0)
		bignum_add_numlst(bignum, carry);
	if (bignum->least == NULL)
		return ;
}

void			bignum_mul_small(t_bignum *bignum, unsigned int num, int count)
{

	unsigned long long	mul;
	int					i;

	i = 0;
	while (count > 0)
	{
		if (i == 0 || i > count)
		{
			i = 0;
			mul = 1;
			while (count > 0 && mul * num < BN_MUL_MAX)
			{
				mul *= num;
				--count;
				i++;
			}
		}
		else
			count -= i;
		mul_cycle(bignum, mul);
		if (bignum->least == NULL)
			return ;
	}
}
