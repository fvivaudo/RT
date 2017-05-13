/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 12:07:38 by momartin          #+#    #+#             */
/*   Updated: 2016/11/15 09:55:10 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1)
{
	size_t	i;
	size_t	k;
	size_t	len;
	char	*s_out;

	if (s1)
	{
		i = 0;
		k = 0;
		len = ft_strlen(s1);
		while ((s1[len - 1] == ' ' || s1[len - 1] == '\t' ||
				s1[len - 1] == '\n') && len)
			len--;
		if (len == 0)
			return (ft_strdup(""));
		while (s1[i] == ' ' || s1[i] == '\t' || s1[i] == '\n')
			i++;
		s_out = (char *)ft_strnew(sizeof(char) * (len - i));
		if (!s_out)
			return (NULL);
		while (i < len)
			s_out[k++] = s1[i++];
		return (s_out);
	}
	return (NULL);
}
