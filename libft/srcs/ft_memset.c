/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 09:51:50 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/07 17:17:32 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memset(void *b, int c, size_t len)
{
	int i;

	i = 0;
	while (len-- > 0)
	{
		((char*)b)[i] = (unsigned char)c;
		++i;
	}
	return (b);
}
