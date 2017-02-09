/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcatf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/06 11:45:58 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/06 11:49:04 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strcatf(const char *s1, char *s2)
{
	int end;
	int i;

	i = ft_strlen(s2);
	end = ft_strlen(s1);
	while (i >= 0)
	{
		s2[end + i] = s2[i];
		--i;
	}
	while (++i < end)
		s2[i] = s1[i];
	return (s2);
}
