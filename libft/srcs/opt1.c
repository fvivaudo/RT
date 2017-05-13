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

char	*opt5(va_list list, const char *format, char *res, t_isize a)
{
	char	*to;
	t_pad	p;

	p.pos.type = 1;
	p.opt = 0;
	if (a.tmp == 1)
		to = ft_octal(va_arg(list, long int));
	else if (ft_strcmp((to = (get_flags(format, a.i))), "") == 0)
		to = ft_octal(va_arg(list, unsigned int));
	else
		to = flags(list, format + a.i, to, &ft_octal);
	p = opt51(to, p, a, format);
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	p.format = format;
	res = opt52(to, res, p, a);
	ft_strdel(&to);
	return (res);
}

t_pad	opt51(char *to, t_pad p, t_isize a, const char *format)
{
	p = ft_getpos(format, to, a.i, p);
	if (ft_isheren(format, '#', a.i) > -1 && to[0] != '0'
		&& (p.pos.max < (int)ft_strlen(to)))
	{
		if ((p.pos.min - 1) > 0)
			p.pos.min -= 1;
		else
			p.pos.min = 0;
		(*a.s) += 1;
	}
	*a.s += p.pos.prec > 0 ? p.pos.min + ft_strlen(to) + p.pos.max :
	p.pos.min + ft_strlen(to);
	if (p.pos.prec == 1 && ft_isheren(format, '#', a.i) > -1
		&& ft_strlen(to) == 0)
		*a.s += 1;
	return (p);
}

char	*opt52(char *to, char *res, t_pad p, t_isize a)
{
	if ((p.pos.prec != 1 && to[0] != '0') || ft_isheren(p.format, '#', a.i) > -1
		|| p.pos.prec == 2 || p.pos.prec == 0)
	{
		if (ft_isheren(p.format, '#', a.i) > -1 && to[0] != '0'
			&& (p.pos.zero == 1 || p.pos.prec == 0 || p.pos.prec == 2))
			res = ft_padorder(p, res, to, 18);
		else
			res = ft_padorder(p, res, to, 5);
	}
	else
	{
		*a.g_size -= ft_strlen(to);
		res = ft_padorder(p, res, "", -1);
	}
	return (res);
}

char	*opt6(va_list list, const char *format, char *res, t_isize a)
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
	*a.s += p.pos.prec > 0 ? p.pos.min + ft_strlen(to) + p.pos.max :
	p.pos.min + ft_strlen(to);
	res = rec_option(list, format + a.i + 1, res, a);
	p.i = a.i;
	p.format = format;
	res = opt61(to, res, p, a);
	ft_strdel(&to);
	return (res);
}

char	*opt61(char *to, char *res, t_pad p, t_isize a)
{
	if ((ft_strlen(to) > 0) || p.pos.prec == 2 || p.pos.prec == 0)
	{
		if (ft_isheren(p.format, '#', a.i) > -1 && (p.pos.zero == 1
			|| (to[0] != '0' && p.pos.prec == 2)))
			res = ft_padorder(p, res, to, 17);
		else
			res = ft_padorder(p, res, to, 6);
	}
	else
	{
		*a.g_size -= ft_strlen(to);
		res = ft_padorder(p, res, "", -1);
	}
	return (res);
}
