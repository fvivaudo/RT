/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 11:01:07 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 13:35:39 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	int i;

	i = ft_strlen(s1);
	while (*s2 != '\0' && n > 0)
	{
		s1[i] = *s2;
		i++;
		s2++;
		n--;
	}
	s1[i] = '\0';
	return (s1);
}
