/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 17:29:59 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 15:44:06 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (s2[0] == 0)
		return ((char*)s1);
	while (s1[i] != '\0' && n > 0)
	{
		while ((s1[i + j] == s2[j]) && ((n - j++) > 0))
			if (s2[j] == '\0')
				return (char *)(s1 + i);
		j = 0;
		n--;
		i++;
	}
	return (NULL);
}
