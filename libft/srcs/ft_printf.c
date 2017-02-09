/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/06 09:47:05 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/06 09:47:07 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

t_pad	ft_getpos(const char *format, char *to_free, int i, t_pad p)
{
	p.pos.max = -1;
	p.pos.min = 0;
	p.pos.zero = 0;
	p.opt = 0;
	get_lnum(format, i, &p.pos);
	get_lprec(format, i, &p.pos);
	p.pos.prec = ft_isheren(format, '.', i) > -1 ? 1 : 0;
	if (p.pos.prec == 1 && ft_atoi(format + p.pos.max) > 0)
		p.pos.prec = 2;
	else if (p.pos.prec == 1 && to_free[0] == '0' && (p.pos.type == 1))
		to_free[0] = '\0';
	if (p.pos.max > -1 && (format[i] == 'c' || format[i] == 'C'))
		p.pos.max = ft_atoi(format + p.pos.max) - 1;
	if (p.pos.max > -1 && (format[i] != 's' && format[i] != 'S'
		&& ((format[i] != 'd' && format[i] != 'i') || to_free[0] != '-')))
		p.pos.max = ft_atoi(format + p.pos.max) - ft_strlen(to_free);
	else if (p.pos.max > -1 && (format[i] == 'd' || format[i] == 'i')
		&& to_free[0] == '-')
		p.pos.max = ft_atoi(format + p.pos.max) - ft_strlen(to_free) + 1;
	else if (p.pos.max > -1 && (format[i] == 's' || format[i] == 'S'))
		p.pos.max = ft_atoi(format + p.pos.max);
	return (ft_getpos2(format, to_free, i, p));
}

t_pad	ft_getpos2(const char *format, char *to_free, int i, t_pad p)
{
	if (p.pos.max < 0)
		p.pos.max = 0;
	if (p.pos.min > -1)
	{
		if ((format[i] == 'c' || format[i] == 'C') && to_free[0] == '\0')
			p.pos.min = ft_atoi(format + p.pos.min) - 1;
		else if (format[i] != 'S' && format[i] != 's')
			p.pos.min = ft_atoi(format + p.pos.min) - ft_strlen(to_free);
		else
			p.pos.min = ft_atoi(format + p.pos.min);
	}
	if (p.pos.type == 1)
		p.pos.min -= p.pos.prec == 0 ? 0 : p.pos.max;
	if (p.pos.min < 0)
		p.pos.min = 0;
	if (ft_isheren(format, '-', i) == -1 && (ft_isheren(format, '.', i) == -1
		|| p.pos.type != 1))
		p.pos.zero = ft_zero(format, i);
	p.c[0] = p.pos.zero == 1 ? '0' : ' ';
	p.c[1] = '\0';
	return (p);
}

char	*ft_padding(char *res, t_prec pos, char *c, int opt)
{
	if (opt == 2 || opt == 3)
		pos.min = pos.min;
	if (pos.type < 2)
	{
		while (pos.min-- > 0)
			res = ft_strcatf(c, res);
	}
	else
		while (pos.min-- > 0)
			res = ft_memcatf(c, res, 1, pos.zero);
	return (res);
}

char	*opt15(va_list list, const char *format, char *res, t_isize a)
{
	char	c;
	t_pad	p;

	p.pos.type = 0;
	p.opt = 0;
	if (format[a.i] != '\0')
	{
		p = ft_getpos(format, " ", a.i, p);
		c = *(format + a.i);
		*a.s += p.pos.min + 1;
		res = rec_option(list, format + a.i + 1, res, a);
	}
	else
		res = rec_option(list, format + a.i, res, a);
	if (format[a.i] == '\0')
		return (res);
	p.i = a.i;
	p.format = format;
	res = ft_padorder(p, res, &c, 20);
	return (res);
}

int		ft_printf(const char *format, ...)
{
	va_list			list;
	char			*ret;
	t_isize			a;
	int				b;
	int				c;

	b = 0;
	c = 0;
	a.s = &c;
	a.g_size = &b;
	a.i = 0;
	ret = NULL;
	ret = ft_strdup("");
	va_start(list, format);
	ret = rec_option(list, format, ret, a);
	va_end(list);
	write(1, ret, *a.g_size);
	ft_strdel(&ret);
	return (*a.g_size);
}
