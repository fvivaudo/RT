/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 13:42:55 by momartin          #+#    #+#             */
/*   Updated: 2016/11/07 12:03:13 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char *save_dst;
	unsigned char *save_src;

	if (!n)
		return (dst);
	save_dst = (unsigned char *)dst;
	save_src = (unsigned char *)src;
	while ((size_t)n--)
		*save_dst++ = *save_src++;
	return (dst);
}
