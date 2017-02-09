/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_root.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/05 14:51:19 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/06/05 14:51:21 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	ft_root(double num)
{
	double	new_ret;
	double	ret;
	double	h;
	double	low;

	ret = 0;
	while ((ret * ret) < num)
		++ret;
	if ((ret * ret) == num)
		return (ret);
	h = ret;
	low = --ret;
	new_ret = -1;
	while (new_ret != ret)
	{
		new_ret = ret;
		if ((ret * ret) < num)
			low = ret;
		else
			h = ret;
		ret = (ret * ret) < num ? ret + (h - low) / 2 : ret - (h - low) / 2;
	}
	return (ret);
}
