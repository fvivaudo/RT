/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/14 13:28:40 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/14 13:28:41 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

char	*opt11(va_list list, const char *format, char *res, t_isize a)
{
	a.tmp = 1;
	return (opt2(list, format, res, a));
}

char	*opt12(va_list list, const char *format, char *res, t_isize a)
{
	wchar_t	tmp;
	char	*to;
	t_pad	p;

	p.pos.type = 0;
	p.opt = 0;
	tmp = va_arg(list, wchar_t);
	if (tmp == 0)
	{
		to = ft_strdup("");
		++(*a.s);
	}
	else
		to = ft_strnew(32);
	opt12b(tmp, to, 0);
	p = ft_getpos(format, to, a.i, p);
	*a.s += p.pos.min + ft_strlen(to);
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	p.format = format;
	res = ft_padorder(p, res, to, 12);
	ft_strdel(&to);
	return (res);
}

int		opt12b(wchar_t tmp, char *to, int y)
{
	int b;

	b = countbit(tmp);
	if (b < 8)
		to[y] = tmp;
	else if (b < 12)
	{
		to[y++] = ((tmp >> 6) + 192);
		to[y] = ((((unsigned char)(tmp)) % 64) + 128);
	}
	else if (b < 17)
	{
		to[y++] = (((unsigned char)(tmp >> (12)) + 224));
		to[y++] = (((unsigned char)(tmp >> (6)) % 64 + 128));
		to[y] = ((((unsigned char)(tmp)) % 64) + 128);
	}
	else
	{
		to[y++] = (((unsigned char)(tmp >> (18)) + 240));
		to[y++] = (((unsigned char)(tmp >> (12)) % 64 + 128));
		to[y++] = (((unsigned char)(tmp >> (6)) % 64 + 128));
		to[y] = ((((unsigned char)(tmp)) % 64) + 128);
	}
	y++;
	return ((char)(y));
}

char	*opt13(va_list list, const char *format, char *res, t_isize a)
{
	a.tmp = 1;
	return (opt5(list, format, res, a));
}

char	*opt14(va_list list, const char *format, char *res, t_isize a)
{
	char	*to;
	t_pad	p;

	p.opt = 0;
	p.pos.type = 1;
	to = ft_uns(va_arg(list, unsigned long int));
	p = ft_getpos(format, to, a.i, p);
	*a.s += p.pos.min + ft_strlen(to) + p.pos.max;
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	p.format = format;
	res = ft_padorder(p, res, to, 14);
	ft_strdel(&to);
	return (res);
}
