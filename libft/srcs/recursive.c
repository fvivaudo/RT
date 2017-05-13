/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/14 13:28:22 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/14 13:28:27 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

char	*rec_option(va_list list, const char *format, char *res, t_isize a)
{
	a.tmp = 0;
	a.i = 0;
	while (*(format + a.i))
	{
		if (*(format + a.i) == '%')
		{
			if (a.i != 0)
			{
				a.tmp = ft_ldiff("#0123456789-+hljtz. ", format + a.i + 1);
				*a.s += a.i;
				res = rec_option(list, format + a.i, res, a);
				if (format[a.i + a.tmp + 1] != 'c'
					&& format[a.i + a.tmp + 1] != 'C')
					res = ft_strncatf(format, res, a.i);
				else
					res = ft_memcatf(format, res, a.i, *a.s);
				return (res);
			}
			else
				return (rec_bis(list, format, res, a));
		}
		a.i++;
	}
	return (rec_option2(format, res, a));
}

char	*rec_option2(const char *format, char *res, t_isize a)
{
	if (a.i != 0)
	{
		res = (char*)malloc(*a.s + a.i);
		ft_strclr(res);
		res = ft_strcpy(res, format);
	}
	else
		res = ft_strnew(*a.s);
	*a.g_size += *a.s + a.i;
	return (res);
}

char	*rec_bis(va_list list, const char *format, char *res, t_isize a)
{
	format++;
	a.i = ft_ldiff("#0123456789-+hljtz. ", format);
	if (a.i == -1)
		a.i = 0;
	if (((format[a.i]) == 'd' || (format[a.i]) == 'i'))
		return (opt2(list, format, res, a));
	else if (format[a.i] == 's')
		return (opt3(list, format, res, a));
	else if (format[a.i] == 'c')
		return (opt4(list, format, res, a));
	else if (format[a.i] == 'o')
		return (opt5(list, format, res, a));
	else if (format[a.i] == 'x')
		return (opt6(list, format, res, a));
	else if (format[a.i] == 'u')
		return (opt7(list, format, res, a));
	else if (format[a.i] == 'p')
		return (opt8(list, format, res, a));
	else if (format[a.i] == 'S')
		return (opt10(list, format, res, a));
	else if (format[a.i] == 'C')
		return (opt12(list, format, res, a));
	return (rec_ter(list, format, res, a));
}

char	*rec_ter(va_list list, const char *format, char *res, t_isize a)
{
	if (format[a.i] == 'X')
		return (opt9(list, format, res, a));
	else if (format[a.i] == 'D')
		return (opt11(list, format, res, a));
	else if (format[a.i] == 'O')
		return (opt13(list, format, res, a));
	else if (format[a.i] == 'U')
		return (opt14(list, format, res, a));
	return (opt15(list, format, res, a));
}

int		countbit(wchar_t tmp)
{
	int i;
	int b;

	b = 1;
	i = tmp;
	while (i > 1)
	{
		i = i / 2;
		++b;
	}
	return (b);
}
