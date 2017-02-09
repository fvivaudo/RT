/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 12:26:42 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/08 13:00:32 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int i;

	if (n == 0)
		return (0);
	i = 0;
	while (n-- > 1 && ((const char*)s1)[i] == ((const char*)s2)[i])
		i++;
	return ((unsigned char)((const char*)s1)[i] -
	(unsigned char)((const char*)s2)[i]);
}
