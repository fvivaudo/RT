/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 16:48:33 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/12/10 11:33:57 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	int		i;

	i = 0;
	while (*s != '\0')
	{
		++s;
		++i;
	}
	while (i > -1)
	{
		if (*s == (char)c)
		{
			return (char*)(s);
		}
		--i;
		--s;
	}
	return (NULL);
}
