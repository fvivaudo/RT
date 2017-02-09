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

char			*ft_padorder(t_pad p, char *res, char *add, int s)
{
	if (s == 15 && (p.pos.zero == 1 || p.pos.max > 0))
		add++;
	if (ft_isheren(p.format, '-', p.i) == -1)
		ft_suborder(p, res, add, s);
	if ((s == 16 || s == 17 || s == 18) && (p.pos.zero == 1 || p.pos.prec > 0))
		s += 100;
	if (s == 8 && (p.pos.zero == 1 || p.pos.prec > 1))
		s += 100;
	if (s == 15 && (p.pos.zero == 1 || p.pos.max > 0))
		s += 100;
	if (s == 2 &&
		(p.pos.zero == 1 || p.pos.max > 0 || p.opt == 2 || p.opt == 3))
		s += 100;
	if ((p.pos.type == 1 && ft_isheren(p.format, '-', p.i) == -1))
	{
		while (p.pos.max-- > 0)
			res = ft_strcatf("0", res);
	}
	res = ft_padding(res, p.pos, p.c, p.opt);
	return (ft_padorder2(p, res, add, s));
}

char			*ft_padorder2(t_pad p, char *res, char *add, int s)
{
	if (p.pos.type == 1 && ft_isheren(p.format, '-', p.i) > -1 &&
		(s == 117 || s == 116 || s == 108))
	{
		res = (s == 117 || s == 116 || s == 108) ? ft_strcatf(add, res) : res;
		while (p.pos.max-- > 0)
			res = ft_strcatf("0", res);
	}
	if (ft_isheren(p.format, '-', p.i) > -1 ||
		(s == 102 || s == 108 || (s >= 115 && s <= 118)))
		ft_suborder(p, res, add, s);
	if (p.pos.type == 1 && ft_isheren(p.format, '-', p.i) > -1)
	{
		while (p.pos.max-- > 0)
			res = ft_strcatf("0", res);
		if (s == 102 && (p.opt == 2 || p.opt == 3))
			res = ft_strncatf("+", res, 1);
	}
	if (p.opt == 1)
	{
		if (res[0] == '0' && p.pos.zero == 1)
			res[0] = ' ';
		else if (res[0] != ' ' && p.pos.type == 1 && res[0] != '-')
			res = ft_strcatf(" ", res);
	}
	return (res);
}

char			*ft_suborder(t_pad p, char *r, char *add, int s)
{
	t_spec	l;

	l.s = s;
	l.ind = 0;
	if (p.pos.min > p.pos.max
		&& ft_isheren(p.format, '-', p.i) == -1 && p.pos.prec == 2)
		l.ind = p.pos.min;
	l.y = ft_strlen(add) > 0 ? (int)ft_strlen(add) : p.pos.min + 1;
	l.x = 0;
	if (ft_isheren(p.format, '.', p.i) > -1
		&& (int)ft_strlen(add) > p.pos.max && (l.s == 3 || l.s == 10))
	{
		p.pos.min += ft_strlen(add);
		p.pos.min -= p.pos.max;
		l.x = 2;
	}
	return (ft_suborder3(p, r, add, l));
}

char			*ft_suborder2(t_pad p, char *r, char *add, t_spec l)
{
	r = l.s == 102 &&
	ft_isheren(p.format, '-', p.i) > -1 ? ft_strcatf(add, r) : r;
	r = l.s == 118 &&
	ft_isheren(p.format, '-', p.i) > -1 ? ft_strcatf(add, r) : r;
	r = (p.opt == 2 || p.opt == 3) &&
	((l.s == 102 && (l.s != 102 || (p.opt != 2 && p.opt != 3)
		|| ft_isheren(p.format, '-', p.i) == -1)
	&& ((p.pos.prec == 2 || ft_isheren(p.format, '-', p.i) > -1)
		|| (p.pos.zero == 1 && p.pos.min > 0)))
	|| (l.s == 2 && ((p.pos.prec == 0 || p.pos.prec == 1)
		&& (p.pos.zero == 0 || p.pos.min == 0)))) ?
	ft_strncatf("+", r + l.ind, 1) : r;
	r = l.x == 1 && p.pos.max == 0 && (l.s == 5 || l.s == 18 || l.s == 118) ?
	ft_strncatf("0", r + l.ind, 1) : r;
	r = ((l.s == 8 && (p.pos.zero == 0 && p.pos.max == 0) &&
		(p.pos.prec == 0 || p.pos.prec == 1)) || l.s == 108)
	? ft_strncatf("0x", r + l.ind, 2) : r;
	r = ((l.s == 6 && l.x == 1) || l.s == 117) && ft_strncmp("0x", r, 2) != 0 ?
	ft_strncatf("0x", r + l.ind, 2) : r;
	r = (l.s == 9 && l.x == 1) || l.s == 116 ? ft_strncatf("0X", r + l.ind, 2) :
	r;
	r = l.s == 115 ? ft_strncatf("-", r + p.pos.min, 1) : r;
	r = l.s == 20 ? ft_strncatf(add, r, 1) : r;
	r = l.s == 4 || (l.s == 12 && l.x == 1) ? ft_strncatf(add, r, 1) : r;
	return (r);
}

char			*ft_suborder3(t_pad p, char *r, char *add, t_spec l)
{
	if ((ft_isheren(p.format, '#', p.i) > -1 && add[0] != '0')
		|| (add[0] == 0 && (l.s == 12)) || (p.pos.prec == 1 && (l.s == 5)))
	{
		l.x = 1;
		if (((l.s == 18 || l.s == 118) && p.pos.max >= (int)ft_strlen(add)))
			l.x = 0;
	}
	r = (l.s == 3 || l.s == 10) && l.x == 2 ? ft_strncatf(add, r, p.pos.max) :
	r;
	r = l.s == 2 || (l.s == 3 && l.x != 2) ||
	((l.s >= 5 && l.s <= 18) && l.x != 2 && l.s != 12) ? ft_strcatf(add, r) : r;
	if (p.pos.min > 0 && ft_isheren(p.format, '-', p.i) == -1)
		r = l.s == 12 ? ft_strncatf(add, r, l.y) : r;
	else
		r = l.s == 12 ? ft_strcatf(add, r) : r;
	return (ft_suborder2(p, r, add, l));
}
