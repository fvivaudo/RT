/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/13 00:05:32 by momartin          #+#    #+#             */
/*   Updated: 2016/11/13 02:17:13 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

long			ft_nbrlen(long n)
{
	long i;
	long nbr;

	i = 1;
	nbr = n;
	if (n < 0)
	{
		nbr = -n;
		i++;
	}
	while (nbr >= 10)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

char			*ft_itoa(long n)
{
	char	*s_out;
	long	nbr;
	long	len;

	if (n == LONG_MIN)
		return (ft_strdup("-9223372036854775808"));
	nbr = n;
	len = ft_nbrlen(n);
	if (!(s_out = ft_strnew(sizeof(char) * len)))
		return (NULL);
	if (n < 0)
		nbr = -n;
	while (len)
	{
		s_out[len - 1] = (nbr % 10) + '0';
		nbr /= 10;
		len--;
	}
	if (n < 0)
		s_out[0] = '-';
	return (s_out);
}
