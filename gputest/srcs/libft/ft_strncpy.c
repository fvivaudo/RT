/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 06:57:14 by momartin          #+#    #+#             */
/*   Updated: 2016/11/07 16:47:17 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *s1, char const *s2, size_t n)
{
	size_t i_s2;

	i_s2 = ft_strlen(s2);
	if (n)
		ft_memcpy(s1, s2, n);
	if (n > i_s2)
	{
		while (i_s2 < n)
		{
			s1[i_s2] = '\0';
			i_s2++;
		}
	}
	return (s1);
}
