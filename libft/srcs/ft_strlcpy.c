/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 16:15:27 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/11 16:40:25 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <string.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	int i;

	i = 0;
	while ((unsigned int)i < size - 1 && src[i] != 0)
	{
		dst[i] = src[i];
		++i;
	}
	dst[i] = '\0';
	return ((unsigned int)ft_strlen(src));
}
