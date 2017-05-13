/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/05 14:49:14 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/06/05 14:49:15 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	ft_pow(double num, int power)
{
	double	ret;

	ret = 1;
	while (power > 0)
	{
		ret *= num;
		--power;
	}
	while (power < 0)
	{
		ret /= num;
		++power;
	}
	return (ret);
}
