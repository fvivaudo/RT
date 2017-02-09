/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/05 16:16:07 by fvivaudo          #+#    #+#             */
/*   Updated: 2016/11/04 16:19:36 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Deal_shadow */

#include <rtv1.h>

void	print_img(unsigned char img[3 * WIDTH * HEIGHT])
{
	t_env		e;
	int			i;
	int			y;

	i = 0;
	y = 0;
	e.mlx = mlx_init();
	e.win = mlx_new_window(e.mlx, WIDTH, HEIGHT, "42");
	while (i < HEIGHT)
	{
		while (y < WIDTH)
		{
			mlx_pixel_put(e.mlx, e.win, y, i, (img[(y + i * WIDTH) * 3 + 2])
			+ (img[(y + i * WIDTH) * 3 + 1] * 256)
			+ (img[(y + i * WIDTH) * 3] * 256 * 256));
			++y;
		}
		y = 0;
		++i;
	}
	mlx_loop(e.mlx);
}

