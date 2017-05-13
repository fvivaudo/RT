/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 10:30:05 by momartin          #+#    #+#             */
/*   Updated: 2016/12/05 18:14:15 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strsub(char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*s_out;

	if (s)
	{
		s_out = ft_strnew(sizeof(char) * len);
		if (!s_out)
			return (NULL);
		i = 0;
		while (i < len)
		{
			s_out[i] = s[start + i];
			i++;
		}
		free(s);
		return (s_out);
	}
	return (NULL);
}
