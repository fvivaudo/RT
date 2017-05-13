/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_u.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 04:03:58 by momartin          #+#    #+#             */
/*   Updated: 2017/01/23 04:03:59 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int		ft_nbrlen(unsigned long n)
{
	unsigned long i;
	unsigned long nbr;

	i = 1;
	nbr = n;
	while (nbr >= 10)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

char					*ft_itoa_u(unsigned long n)
{
	char			*s_out;
	unsigned long	nbr;
	unsigned long	len;

	nbr = n;
	len = ft_nbrlen(n);
	if (!(s_out = ft_strnew(sizeof(char) * len)))
		return (NULL);
	while (len)
	{
		s_out[len - 1] = (nbr % 10) + '0';
		nbr /= 10;
		len--;
	}
	return (s_out);
}
