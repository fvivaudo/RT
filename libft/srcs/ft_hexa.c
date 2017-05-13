/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 18:13:15 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 13:23:44 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

char	*ft_hexa(unsigned long long int n)
{
	unsigned long long int	puis;
	int						nb_char;
	char					*res;

	nb_char = 0;
	puis = 1;
	while ((n / puis) > 15)
	{
		nb_char++;
		puis = puis * 16;
	}
	res = malloc(nb_char + 1);
	if (res == NULL)
		return (NULL);
	while (puis != 1)
	{
		*res = n / puis < 10 ? n / puis + '0' : n / puis + 87;
		res++;
		n = n % puis;
		puis = puis / 16;
	}
	*res = n / puis < 10 ? n / puis + '0' : n / puis + 87;
	res++;
	*res = '\0';
	return (res - nb_char - 1);
}
