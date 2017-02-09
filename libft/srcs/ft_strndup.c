/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/16 10:35:05 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/16 10:46:52 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

char	*ft_strndup(char *src, size_t n)
{
	int		i;
	char	*done;

	i = 0;
	done = (char*)malloc(sizeof(char) * n + 1);
	if (done == NULL)
		return (NULL);
	while (src[i] != '\0' && (unsigned int)i != n)
	{
		done[i] = src[i];
		++i;
	}
	done[i] = '\0';
	return (done);
}
