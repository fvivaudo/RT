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

char	*opt4(va_list list, const char *format, char *res, t_isize a)
{
	char	*to;
	char	b;
	t_pad	p;

	p.pos.type = 0;
	if (ft_strcmp((to = (get_flags(format, a.i))), "l") == 0)
		return (opt12(list, format, res, a));
	p = ft_getpos(format, " ", a.i, p);
	b = va_arg(list, int);
	*a.s += p.pos.min + 1;
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	p.opt = 0;
	p.format = format;
	p.pos.zero = *a.s;
	p.pos.type = 2;
	res = ft_padorder(p, res, &b, 4);
	return (res);
}

char	*opt91(t_pad p, char *res, t_isize a, char *to)
{
	if ((ft_strlen(to) > 0) || p.pos.prec == 2 || p.pos.prec == 0)
	{
		if (ft_isheren(p.format, '#', a.i) > -1 && (p.pos.zero == 1
			|| (to[0] != '0' && p.pos.prec == 2)))
			res = ft_padorder(p, res, to, 16);
		else
			res = ft_padorder(p, res, to, 9);
	}
	else
	{
		*a.g_size -= ft_strlen(to);
		res = ft_padorder(p, res, "", -1);
	}
	return (res);
}

char	*opt10(va_list list, const char *format, char *res, t_isize a)
{
	wchar_t	*tmp;
	t_op10	o;
	t_pad	p;

	p.pos.type = 0;
	o.total = 0;
	p.opt = 0;
	o.y = 0;
	tmp = va_arg(list, wchar_t *);
	o = opt103(o, tmp);
	p = ft_getpos(format, o.to, a.i, p);
	p = opt102(a, o, p);
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	p.format = format;
	res = ft_padorder(p, res, o.to, 10);
	ft_strdel(&(o.to));
	if (tmp)
		ft_strdel(&(o.s_n));
	return (res);
}

t_pad	opt102(t_isize a, t_op10 o, t_pad p)
{
	if (p.pos.prec > 0)
	{
		if ((int)ft_strlen(o.to) > p.pos.max)
		{
			while (o.total > p.pos.max)
				o.total -= (o.s_n)[o.y--];
			(o.to)[o.total] = '\0';
		}
		if (p.pos.max > o.total)
			p.pos.max = o.total;
		p.pos.min -= o.total;
		if (p.pos.min < 0)
			p.pos.min = 0;
		*a.s += p.pos.min + p.pos.max;
	}
	else
	{
		p.pos.min -= ft_strlen(o.to);
		if (p.pos.min < 0)
			p.pos.min = 0;
		*a.s += p.pos.min + ft_strlen(o.to);
	}
	return (p);
}

t_op10	opt103(t_op10 o, wchar_t *tmp)
{
	if (tmp)
	{
		while (tmp[o.y])
			o.y++;
		o.to = ft_strnew(o.y * 4);
		o.s_n = ft_strnew(o.y);
		o.y = 0;
		while (tmp[o.y])
		{
			(o.s_n)[o.y] = opt12b(tmp[o.y], o.to + ft_strlen(o.to), 0);
			o.total += (o.s_n)[o.y];
			(o.y)++;
		}
		(o.y)--;
	}
	else
		o.to = ft_strdup("(null)");
	return (o);
}
