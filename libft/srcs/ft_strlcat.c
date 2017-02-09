/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 11:13:20 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/11 16:12:37 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <libft.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	int i;
	int lim;
	int end;

	lim = ft_strlen(dst);
	if (size < (unsigned int)lim)
		end = (ft_strlen(src) + size);
	else
		end = lim + ft_strlen(src);
	i = lim;
	while (*src != '\0' && size > (unsigned int)lim + 1)
	{
		dst[i++] = *src;
		src++;
		size--;
	}
	dst[i] = '\0';
	return (end);
}
