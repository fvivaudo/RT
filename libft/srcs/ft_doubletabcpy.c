/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_doubletabcpy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/13 18:41:58 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/03/13 18:42:06 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <strings.h>
#include <stdlib.h>

char		**ft_doubletabcpy(char **src)
{
	int		i;
	char	**dest;

	i = 0;
	while (src[i])
		++i;
	dest = (char**)malloc((i + 1) * sizeof(char*));
	*dest = NULL;
	dest[i + 1] = NULL;
	while (--i > -1)
		dest[i] = ft_strdup((src)[i]);
	return (dest);
}
