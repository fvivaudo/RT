/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/14 13:28:50 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/14 13:28:51 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

char	*opt7(va_list list, const char *format, char *res, t_isize a)
{
	char	*to;
	t_pad	p;

	p.opt = 0;
	p.pos.type = 1;
	if (ft_strcmp((to = (get_flags(format, a.i))), "") == 0)
		to = ft_uns(va_arg(list, unsigned int));
	else
		to = flags(list, format + a.i, to, &ft_uns);
	p = ft_getpos(format, to, a.i, p);
	*a.s += p.pos.prec > 0 ? p.pos.min + ft_strlen(to) + p.pos.max
	: p.pos.min + ft_strlen(to);
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	p.format = format;
	if ((p.pos.prec > 0 && to[0] != '0') || ft_isheren(format, '.', a.i) == -1)
		res = ft_padorder(p, res, to, 7);
	else
	{
		*a.g_size -= ft_strlen(to);
		res = ft_padorder(p, res, "", 7);
	}
	ft_strdel(&to);
	return (res);
}

char	*opt8(va_list list, const char *format, char *res, t_isize a)
{
	char	*to;
	t_pad	p;

	p.y = 0;
	p.opt = 0;
	p.pos.type = 1;
	to = NULL;
	to = ft_hexa(va_arg(list, unsigned long));
	p = ft_getpos(format, to, a.i, p);
	if ((p.pos.min - 2) > 0)
		p.pos.min -= 2;
	else
		p.pos.min = 0;
	if (ft_isheren(format, '.', a.i) > -1)
		p.pos.max = (p.pos.max - p.y) > 0 ? p.pos.max - p.y : 0;
	*a.s += p.pos.min + ft_strlen(to) + p.y + 2;
	if (p.pos.prec > 0)
		*a.s += p.pos.max;
	res = rec_option(list, format + a.i + 1, res, a);
	if (ft_strlen(to) > 16)
		to = ft_strndup(to, 16);
	p.format = format;
	res = opt81(p, res, a, to);
	ft_strdel(&to);
	return (res);
}

char	*opt81(t_pad p, char *res, t_isize a, char *to)
{
	p.i = a.i;
	if ((p.pos.prec > 0 && to[0] != '0')
		|| (ft_isheren(p.format, '.', a.i) == -1) || p.pos.max > 0)
	{
		if (to[0] == '-' && (p.pos.zero == 1)
			&& (ft_isheren(p.format, '-', a.i)) == -1)
		{
			to[0] = '0';
			p.y = 1;
		}
		res = ft_padorder(p, res, to, 8);
	}
	else
	{
		*a.g_size -= ft_strlen(to);
		(*a.g_size)++;
		res = ft_padorder(p, res, "0", 8);
	}
	return (res);
}

char	*opt9(va_list list, const char *format, char *res, t_isize a)
{
	char	*to;
	t_pad	p;

	p.opt = 0;
	p.pos.type = 1;
	if (ft_strcmp((to = (get_flags(format, a.i))), "") == 0)
		to = ft_hexa(va_arg(list, unsigned int));
	else
		to = flags(list, format + a.i, to, &ft_hexa);
	p = ft_getpos(format, to, a.i, p);
	if (ft_isheren(format, '#', a.i) > -1 && to[0] != '0' && ft_strlen(to) > 0)
	{
		p.pos.min = (p.pos.min - 2) > 0 ? p.pos.min - 2 : 0;
		(*a.s) += 2;
	}
	*a.s += p.pos.prec > 0 ? p.pos.min + ft_strlen(to) + p.pos.max
	: p.pos.min + ft_strlen(to);
	to = ft_strup(to);
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	p.format = format;
	res = opt91(p, res, a, to);
	ft_strdel(&to);
	return (res);
}
