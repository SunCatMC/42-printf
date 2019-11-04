#include "libft.h"
#include "colors.h"
#include "ft_printf.h"

static void	printf_color(t_pbuff *pbuff, const char **fmt, int type,
															int color_id)
{
	if (color_id < 8)
	{
		putstr_pbuff(pbuff, "\x1b[");
		putchar_pbuff(pbuff, type);
		putchar_pbuff(pbuff, '0' + color_id);
		putchar_pbuff(pbuff, 'm');
	}
	else if (color_id == 8)
		putstr_pbuff(pbuff, "\x1b[0m");
	if (color_id <= 8)
		*fmt = ft_strchr(*fmt, '}') + 1;
	else
		putchar_pbuff(pbuff, '{');
}

void		parse_colors(t_pbuff *pbuff, const char **fmt)
{
	int		i;
	int		ch;

	ch = '3';
	if (**fmt == '#')
	{
		ch = '9';
		++*fmt;
	}
	i = 0;
	while (i <= 8 && ft_strsubcmp(*fmt, g_color_codes[i]))
		++i;
	printf_color(pbuff, fmt, ch, i);
}
