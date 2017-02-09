/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lndiff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/14 13:32:19 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/14 13:32:20 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_lndiff(char *ignore, const char *searched, int n)
{
	int i;
	int y;

	i = 0;
	while (searched[i] && i < n)
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
