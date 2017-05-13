/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 17:59:21 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/08 12:28:01 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_putnbr(int nb)
{
	if ((unsigned int)nb > 2147483648 && nb < 0)
	{
		ft_putchar('-');
		nb = nb * (-1);
	}
	if (nb > 9 || nb < -9)
		ft_putnbr(nb / 10);
	ft_putchar(((unsigned int)nb % 10) + '0');
}
