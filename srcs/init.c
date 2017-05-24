/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:57:51 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 09:34:38 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* init objets + lights */

#include <rtv1.h>
#include <stdio.h>
#include <math.h>

void				readConfig2(char *filename)
{
	t_parser r;

	r.comp_id = 1;
	r.fi = fopen("confage", "w");
	r.content = get_file_content(filename);
	read_check(&r);
	read_work(&r);
	free_tab(r.tmpfree);
	free(r.content);
	fclose(r.fi);
}

void    init_display(t_display *e)
{
	SDL_Init(SDL_INIT_VIDEO);
    e->window = SDL_CreateWindow("RT", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    e->render = SDL_CreateRenderer(e->window, -1, SDL_RENDERER_ACCELERATED);
    e->view = SDL_CreateTexture(e->render, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    e->pxl_view = (Uint32*)(malloc(sizeof(Uint32) * WIDTH * HEIGHT));
	e->surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32 ,0 ,0 ,0 ,0);
	ft_memset(e->pxl_view, 0, sizeof(Uint32) * WIDTH * HEIGHT );
}

t_color		colorinit(double red, double green, double blue)
{
	t_color	col;

	col.red = red;
	col.green = green;
	col.blue = blue;
	return (col);
}
