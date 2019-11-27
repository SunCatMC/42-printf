#include "libft.h"
#include "ft_printf.h"

static const char	*get_color(int color_id)
{
	const char *str;
	const char *color_codes[] = {
		"black}",
		"red}",
		"green}",
		"yellow}",
		"blue}",
		"magenta}",
		"cyan}",
		"white}",
		"eoc}"
	};

	str = color_codes[color_id];
	return (str);
}

static void			printf_color(t_pbuff *pbuff, const char **fmt, int type,
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

void				parse_colors(t_pbuff *pbuff, const char **fmt)
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
	while (i <= 8 && ft_strsubcmp(*fmt, get_color(i)))
		++i;
	printf_color(pbuff, fmt, ch, i);
}
