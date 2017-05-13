/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 09:14:06 by momartin          #+#    #+#             */
/*   Updated: 2016/11/09 09:41:19 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	size_t	len;
	size_t	i;
	char	*s_out;

	if (s)
	{
		i = 0;
		len = ft_strlen(s);
		s_out = ft_strnew(sizeof(char) * len);
		if (!s_out)
			return (NULL);
		while (s[i])
		{
			s_out[i] = f(s[i]);
			i++;
		}
		return (s_out);
	}
	return (NULL);
}
