/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/07/11 10:28:45 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/09 11:46:32 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <string.h>

char	*ft_strdup(char *src)
{
	int		i;
	char	*done;

	i = 0;
	if (src == NULL)
		return (NULL);
	done = (char*)malloc(sizeof(char) * ft_strlen(src) + 1);
	if (done == NULL)
		return (NULL);
	while (src[i] != '\0')
	{
		done[i] = src[i];
		i++;
	}
	done[i] = '\0';
	return (done);
}
