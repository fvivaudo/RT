/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ldiff.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/14 13:32:10 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/14 13:32:14 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_ldiff(char *ignore, const char *searched)
{
	int i;
	int y;

	i = 0;
	while (searched[i])
	{
		y = 0;
		while (ignore[y] && ignore[y] != searched[i])
			y++;
		if (ignore[y] == '\0')
			return (i);
		i++;
	}
	return (-1);
}
