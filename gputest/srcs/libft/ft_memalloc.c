/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 07:09:09 by momartin          #+#    #+#             */
/*   Updated: 2016/11/06 13:00:44 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_memalloc(size_t size)
{
	void			*zone;

	if (!size)
		return (NULL);
	zone = malloc(size);
	if (!zone)
		return (NULL);
	zone = ft_memset(zone, 0, size);
	return (zone);
}
