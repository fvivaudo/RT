/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_octal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 18:13:15 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 13:23:44 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

char	*ft_octal(unsigned long long int n)
{
	unsigned long long int		puis;
	int							nb_char;
	char						*res;

	nb_char = 0;
	puis = 1;
	while ((n / puis) > 7)
	{
		++nb_char;
		puis = puis * 8;
	}
	res = malloc(nb_char + 1);
	if (res == NULL)
		return (NULL);
	while (puis != 1)
	{
		*res = n / puis + '0';
		++res;
		n = n % puis;
		puis = puis / 8;
	}
	*res = n / puis + '0';
	++res;
	*res = '\0';
	return (res - nb_char - 1);
}
