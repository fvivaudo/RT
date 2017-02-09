/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 11:42:13 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 11:11:43 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	int		i;
	char	cpy[len];

	i = 0;
	while ((unsigned int)i != len)
	{
		cpy[i] = ((const char *)src)[i];
		i++;
	}
	i = 0;
	while (len-- > 0)
	{
		((char*)dst)[i] = ((const char*)cpy)[i];
		i++;
	}
	return ((char*)dst);
}
