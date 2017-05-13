/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 11:26:04 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 09:31:15 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	int i;

	i = 0;
	while (n-- > 0)
	{
		((unsigned char*)dst)[i] = ((const unsigned char*)src)[i];
		if (((const unsigned char*)src)[i] == (unsigned char)c)
			return (&((char*)dst)[i + 1]);
		i++;
	}
	return (NULL);
}
