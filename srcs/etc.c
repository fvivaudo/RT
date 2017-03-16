/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/05 16:16:07 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:59:11 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Deal_shadow */

#include <rtv1.h>

typedef struct 		s_display
{
	SDL_Window      *window;
	SDL_Renderer    *render;
	SDL_Texture     *view;
	SDL_Surface 	*surface;
	Uint32          *pxl_view;

}					t_display;
/* OLD VERSION MLX
void	back_print_img(unsigned char img[3 * WIDTH * HEIGHT])
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
*/
void    init_display(t_display *e)
{

	SDL_Init(SDL_INIT_VIDEO);
    e->window = SDL_CreateWindow("RT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    e->render = SDL_CreateRenderer(e->window, -1, SDL_RENDERER_ACCELERATED);
    e->view = SDL_CreateTexture(e->render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    e->pxl_view = (Uint32*)(malloc(sizeof(Uint32) * WIDTH * HEIGHT));
	e->surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32 ,0 ,0 ,0 ,0);
	ft_memset(e->pxl_view, 0,sizeof(Uint32) * WIDTH * HEIGHT );

}
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
			draw(&e, x, y, color);
			put_pixel32(e.surface, x, y, color);
		}
	pack(&e);
	SDL_SaveBMP(e.surface, "out.bmp");
	//SDL_Delay(2000);
}

void	swapdouble(double *a, double *b)
{
	double c;

	c = *a;
	*a = *b;
	*b = c;
}
