/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/14 13:28:16 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/14 13:28:18 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

int		ft_zero(const char *f, int y)
{
	int i;
	int z;
	int u;

	u = 0;
	while (u < y && (z = ft_ldiff("123456789", f + u)) > -1)
	{
		u += z + 1;
		i = u - 2;
		if (i == -1 && f[i + 1] == '0')
			return (1);
		while (i > -1)
		{
			if (((f[i] < '0' || f[i] > '9') && f[i] != '.') && f[i + 1] == '0')
				return (1);
			i--;
		}
	}
	return (0);
}

void	get_lnum(const char *format, int y, t_prec *pos)
{
	int i;
	int z;
	int u;
	int t;

	u = 0;
	while (u < y && (z = ft_ldiff("0123456789", format + u)) > -1)
	{
		u += z + 1;
		i = u - 2;
		while (i > -1)
		{
			if ((format[i] < '0' || format[i] > '9') && format[i] != '.')
			{
				t = 0;
				++i;
				while (format[i + t] == '0' && (i + t) < (u - 2))
					t++;
				if (format[i + t] > '0' && format[i + t] <= '9')
					pos->min = i + t;
				break ;
			}
			i--;
		}
	}
}

void	get_lprec(const char *form, int y, t_prec *pos)
{
	int i;
	int z;
	int u;

	u = 0;
	while (u < y && (z = ft_ldiff("0123456789", form + u)) > -1)
	{
		u += z + 1;
		i = u - 2;
		while (i > -1)
		{
			if (form[i] == '.')
			{
				pos->max = i + 1;
				break ;
			}
			i--;
		}
	}
}
