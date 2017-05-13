/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/14 13:28:10 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/14 13:28:11 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

char	*flags(va_list list, const char *format, char *flg, char *(*gen_res)())
{
	if (ft_strcmp(flg, "h") == 0)
	{
		if (ft_ishere("di", format[0]) > -1)
			return (ft_itoa((short)va_arg(list, int)));
		else if (ft_ishere("ouxX", format[0]) > -1)
			return (gen_res((unsigned short)va_arg(list, unsigned int)));
	}
	if (ft_strcmp(flg, "l") == 0)
	{
		if (ft_ishere("di", format[0]) > -1)
			return (ft_litoa(va_arg(list, signed long)));
		else if (ft_ishere("ouxX", format[0]) > -1)
			return (gen_res(va_arg(list, unsigned long)));
	}
	if (ft_strcmp(flg, "2") == 0)
	{
		if (ft_ishere("di", format[0]) > -1)
			return (ft_llitoa(va_arg(list, signed long long)));
		else if (ft_ishere("ouxX", format[0]) > -1)
			return (gen_res(va_arg(list, unsigned long long int)));
	}
	return (flags2(list, format, flg, gen_res));
}

char	*flags2(va_list list, const char *format, char *flg, char *(*gen_res)())
{
	if (ft_strcmp(flg, "1") == 0)
	{
		if (ft_ishere("di", format[0]) > -1)
			return (ft_chitoa((unsigned char)(va_arg(list, int))));
		else if (ft_ishere("ouxX", format[0]) > -1)
			return (gen_res((unsigned char)(va_arg(list, unsigned int))));
	}
	if (ft_strcmp(flg, "j") == 0)
	{
		if (ft_ishere("di", format[0]) > -1)
			return (ft_litoa(va_arg(list, intmax_t)));
		else if (ft_ishere("ouxX", format[0]) > -1)
			return (gen_res(va_arg(list, uintmax_t)));
	}
	if (ft_strcmp(flg, "z") == 0)
	{
		if (ft_ishere("di", format[0]) > -1)
			return (ft_llitoa(va_arg(list, ssize_t)));
		else if (ft_ishere("ouxX", format[0]) > -1)
			return (gen_res(va_arg(list, size_t)));
	}
	return ("");
}

char	*get_flags(const char *format, int i)
{
	int a;

	if ((a = ft_ncount(format, 'l', i)) % 2 == 1)
		return ("l");
	if ((a = ft_ncount(format, 'l', i)) % 2 == 0 && a > 0)
		return ("2");
	if ((a = ft_ncount(format, 'z', i)) % 2 == 1)
		return ("z");
	if ((a = ft_ncount(format, 'j', i)) % 2 == 1)
		return ("j");
	if ((a = ft_ncount(format, 'h', i)) % 2 == 1)
		return ("h");
	if ((a = ft_ncount(format, 'h', i)) % 2 == 0 && a > 0)
		return ("1");
	return ("");
}
