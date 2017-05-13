/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 12:13:12 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 09:39:23 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	int i;

	i = 0;
	while (n-- > 0)
	{
		if (((const unsigned char*)s)[i] == (unsigned char)c)
			return (&((char*)s)[i]);
		i++;
	}
	return (NULL);
}
