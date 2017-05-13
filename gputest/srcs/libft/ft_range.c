/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 11:42:09 by exam              #+#    #+#             */
/*   Updated: 2016/11/15 15:18:08 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int		*ft_range(int start, int end)
{
	long	len;
	int		i;
	int		*table;
	char	incr;

	i = 0;
	incr = 1;
	len = (end - start) + 1;
	if (len < 0)
	{
		incr = -1;
		len = (start - end) + 1;
	}
	table = (int*)malloc(sizeof(int) * len);
	if (!table)
		return (NULL);
	while (len--)
	{
		table[i] = start + incr * i;
		i++;
	}
	return (table);
}
