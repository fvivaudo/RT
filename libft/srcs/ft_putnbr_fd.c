/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 18:05:28 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/07 16:49:01 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
	{
		n = n * (-1);
		ft_putchar_fd('-', fd);
	}
	if (n > 9)
		ft_putnbr(n / 10);
	ft_putchar_fd((n % 10) + '0', fd);
}
