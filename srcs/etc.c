/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/05 16:16:07 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/17 16:31:30 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Deal_shadow */

#include <rtv1.h>


void draw(t_display *e, int x, int y, int color)
{
	e->pxl_view[y * WIDTH + x] = color;
}

void pack(t_display *e)
{
	SDL_UpdateTexture(e->view, NULL, e->pxl_view, WIDTH * sizeof(Uint32));
	SDL_RenderCopy(e->render, e->view, NULL, NULL);
	SDL_RenderPresent(e->render);
}

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[ ( y * surface->w ) + x ] = pixel;
}

void	print_img(unsigned char img[3 * WIDTH * HEIGHT])
{
	t_display e;
	int y;
	int x;
	int color;

	init_display(&e);
	y = -1;
	while (++y < HEIGHT && (x = -1) != -2)
		while (++x < WIDTH)
		{
			color = (img[(x + y * WIDTH) * 3 + 2])
					+ (img[(x + y * WIDTH) * 3 + 1] * 256)
					+ (img[(x + y * WIDTH) * 3] * 256 * 256);
			put_pixel32(e.surface, x, y, color);
		}
	pack(&e);
	SDL_SaveBMP(e.surface, "out.bmp");
}

void	swapdouble(double *a, double *b)
{
	double c;
	c = *a;
	*a = *b;
	*b = c;
}
