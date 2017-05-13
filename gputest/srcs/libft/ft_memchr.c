/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:44:32 by momartin          #+#    #+#             */
/*   Updated: 2016/11/09 10:40:48 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char *tmp;

	if (n)
	{
		tmp = (unsigned char *)s;
		while (n--)
		{
			if (*tmp == (unsigned char)c)
				return (tmp);
			tmp++;
		}
	}
	return (NULL);
}
