/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 10:54:32 by momartin          #+#    #+#             */
/*   Updated: 2016/12/05 18:13:51 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*s_out;

	if (s1 && s2)
	{
		len_s1 = ft_strlen(s1);
		len_s2 = ft_strlen(s2);
		s_out = (char *)ft_memalloc(sizeof(char) * ((len_s1 + len_s2) + 1));
		if (!s_out)
			return (NULL);
		s_out = ft_strcpy(s_out, (char*)s1);
		s_out = ft_strcat(s_out, (char*)s2);
		return (s_out);
	}
	return (NULL);
}

char	*ft_str_join_free(char *str1, char *str2, char nbr)
{
	char *neo;

	neo = ft_strjoin(str1, str2);
	if (nbr == 1)
		free(str1);
	if (nbr == 2)
		free(str2);
	return (neo);
}
