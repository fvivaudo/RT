/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 09:29:40 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 13:34:30 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	int i;

	i = 0;
	while ((unsigned int)i < n && src != 0)
	{
		if (*src == '\0')
			while ((unsigned int)i < n)
			{
				dst[i] = '\0';
				++i;
			}
		dst[i] = *src;
		++src;
		++i;
	}
	return (dst);
}
