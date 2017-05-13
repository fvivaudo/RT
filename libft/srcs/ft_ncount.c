/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ncount.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/07 09:39:44 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/28 10:33:47 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int		ft_ncount(char const *str, char c, int n)
{
	int		i;
	int		y;

	i = 0;
	y = 0;
	while (str[i] != '\0' && i < n)
	{
		if (str[i] == c)
			y++;
		i++;
	}
	return (y);
}
