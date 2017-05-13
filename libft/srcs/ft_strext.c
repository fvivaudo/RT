/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strext.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/01 12:49:29 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/02/01 13:02:49 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char		*ft_strext(char *s, char *ext, int pos)
{
	char *del[2];

	del[0] = s;
	del[1] = ext;
	s = ft_strjoin(s, ext);
	if (pos == 2)
	{
		ft_strdel(&del[0]);
		ft_strdel(&del[1]);
	}
	else
		ft_strdel(&del[pos]);
	return (s);
}
