/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt7.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/14 13:39:29 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/14 13:39:30 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

char	*opt2(va_list list, const char *format, char *res, t_isize a)
{
	char	*to;
	t_pad	p;

	p.y = 0;
	p.pos.type = 1;
	if (a.tmp == 1)
		to = ft_litoa(va_arg(list, long int));
	else if (ft_strcmp((to = (get_flags(format, a.i))), "") == 0)
		to = ft_itoa(va_arg(list, int));
	else
		to = flags(list, format + a.i, to, &ft_itoa);
	p = ft_getpos(format, to, a.i, p);
	p.format = format;
	p = opt21(to, p, a);
	*a.s += p.pos.prec > 0 ? p.pos.min + ft_strlen(to) + p.pos.max :
	p.pos.min + ft_strlen(to);
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	res = opt22(to, res, p, a);
	ft_strdel(&to);
	return (res);
}

t_pad	opt21(char *to, t_pad p, t_isize a)
{
	if (ft_isheren(p.format, ' ', a.i) > -1)
		p.opt += 1;
	if (to[0] != '-' && (ft_isheren(p.format, '+', a.i) > -1))
		p.opt += 2;
	if ((ft_isheren(p.format, '+', a.i) > -1
		|| ft_isheren(p.format, ' ', a.i) > -1)
		&& p.pos.min == 0 && to[0] != '-')
		(*a.s)++;
	else if ((ft_isheren(p.format, '+', a.i) > -1
		&& p.pos.min > 0 && to[0] != '-'))
	{
		--p.pos.min;
		(*a.s)++;
	}
	if ((ft_isheren(p.format, ' ', a.i) > -1) && p.pos.min > 0 && to[0] != '-'
		&& ft_isheren(p.format, '-', a.i) > -1
		&& ft_isheren(p.format, '+', a.i) == -1)
	{
		--p.pos.min;
		(*a.s)++;
	}
	return (p);
}

char	*opt22(char *to, char *res, t_pad p, t_isize a)
{
	if (p.pos.prec == 0 || to[0] != '0' || p.pos.prec == 2)
	{
		if (to[0] == '-' && (p.pos.zero == 1)
			&& (ft_isheren(p.format, '-', a.i)) == -1)
		{
			to[0] = '0';
			p.y = 1;
		}
		res = to[0] == '-' ? ft_padorder(p, res, to, 15) :
		ft_padorder(p, res, to, 2);
	}
	else
	{
		*a.g_size -= ft_strlen(to);
		res = ft_padorder(p, res, "", 2);
	}
	if (p.y == 1 && (p.pos.zero == 1) && (ft_isheren(p.format, '-', a.i)) == -1)
		res[0] = '-';
	return (res);
}

char	*opt3(va_list list, const char *format, char *res, t_isize a)
{
	char	*tm;
	t_pad	p;

	p.pos.type = 0;
	if (ft_strcmp((tm = (get_flags(format, a.i))), "l") == 0)
		return (opt10(list, format, res, a));
	tm = ft_strdup(va_arg(list, char *));
	if (tm == NULL)
		tm = ft_strdup("(null)");
	p = ft_getpos(format, tm, a.i, p);
	p = opt31(tm, p, a);
	res = rec_option(list, format + a.i + 1, res, a);
	p.format = format;
	p.i = a.i;
	res = ft_padorder(p, res, tm, 3);
	ft_strdel(&tm);
	return (res);
}

t_pad	opt31(char *tm, t_pad p, t_isize a)
{
	if (p.pos.prec == 2 && p.pos.max >= (int)ft_strlen(tm))
		p.pos.max = ft_strlen(tm);
	if (p.pos.prec == 0)
	{
		p.pos.min -= ft_strlen(tm);
		if (p.pos.min < 0)
			p.pos.min = 0;
		*a.s += p.pos.min + ft_strlen(tm);
	}
	else
	{
		p.pos.min = p.pos.min - p.pos.max > 0 ? p.pos.min - p.pos.max : 0;
		*a.s += p.pos.min + p.pos.max;
	}
	return (p);
}
