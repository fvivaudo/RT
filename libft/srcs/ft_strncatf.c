/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncatf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/06 13:16:20 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/06 13:25:52 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strncatf(const char *s1, char *s2, int n)
{
	int end;
	int i;

	i = ft_strlen(s2);
	end = n;
	while (i >= 0)
	{
		s2[end + i] = s2[i];
		--i;
	}
	while (n-- > 0)
	{
		++i;
		s2[i] = s1[i];
	}
	return (s2);
}
