/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy_bck.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:31:38 by momartin          #+#    #+#             */
/*   Updated: 2016/11/07 13:36:57 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy_bck(void *dst, const void *src, size_t n)
{
	unsigned char *save_dst;
	unsigned char *save_src;

	if (!n)
		return (dst);
	save_dst = (unsigned char *)dst;
	save_src = (unsigned char *)src;
	while ((size_t)n--)
		save_dst[n] = save_src[n];
	return (dst);
}
