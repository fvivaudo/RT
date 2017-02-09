/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/06 10:24:25 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 16:32:25 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*res;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	res = (char*)malloc(len + 1);
	if (res == NULL)
		return (NULL);
	while (len-- > 0)
		res[i++] = s[start++];
	res[i] = '\0';
	return (res);
}
