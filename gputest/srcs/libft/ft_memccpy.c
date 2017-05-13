/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 09:00:31 by momartin          #+#    #+#             */
/*   Updated: 2016/11/09 10:41:17 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char *save_dst;
	unsigned char *save_src;

	if (!n)
		return (NULL);
	save_dst = (unsigned char *)dst;
	save_src = (unsigned char *)src;
	while (n--)
	{
		if ((*save_dst++ = *save_src++) == (unsigned char)c)
			return (save_dst);
	}
	return (NULL);
}
