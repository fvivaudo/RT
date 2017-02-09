/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/06 10:42:35 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/16 18:29:23 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <string.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	int		i;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	res = (char*)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (res == NULL)
		return (NULL);
	while (*s1 != '\0')
	{
		res[i++] = *s1;
		++s1;
	}
	while (*s2 != '\0')
	{
		res[i++] = *s2;
		++s2;
	}
	res[i] = '\0';
	return (res);
}
