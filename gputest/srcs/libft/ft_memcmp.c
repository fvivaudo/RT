/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 14:18:18 by momartin          #+#    #+#             */
/*   Updated: 2016/11/09 10:42:04 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s_1;
	unsigned char	*s_2;
	size_t			i;

	if (n)
	{
		i = 0;
		s_1 = (unsigned char*)s1;
		s_2 = (unsigned char*)s2;
		while (n--)
		{
			if (s_1[i] != s_2[i])
				return (s_1[i] - s_2[i]);
			i++;
		}
	}
	return (0);
}
