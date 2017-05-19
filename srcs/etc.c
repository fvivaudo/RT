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

# define IMG_RED(X,Y) img[(X + (Y * WIDTH)) * 3]
# define IMG_GREEN(X,Y) img[(X + ((Y) * WIDTH)) * 3 + 1]
# define IMG_BLUE(X,Y) img[(X + ((Y) * WIDTH)) * 3 + 2]
# define IMG2_RED(X,Y) img2[(X + ((Y) * WIDTH)) * 3]
# define IMG2_GREEN(X,Y) img2[(X + ((Y) * WIDTH)) * 3 + 1]
# define IMG2_BLUE(X,Y) img2[(X + ((Y) * WIDTH)) * 3 + 2]
# define IMG3_BLUE(X,Y) img3[(X + ((Y) * WIDTH)) * 3]

#define ABS(X) ((X)<0 ? -(X) : (X))


void effect_sepia(unsigned char *img)
{
	int y;
	int x;
	int rgb[3];

	y = -1;
	while (++y < HEIGHT && (x = -1))
		while (++x < WIDTH)
		{
			rgb[0] = (int)(0.393 * IMG_RED(x,y) + 0.769 * IMG_GREEN(x,y) + 0.189
			* IMG_BLUE(x,y));
			rgb[1] = (int)(0.349 * IMG_RED(x,y) + 0.686 * IMG_GREEN(x,y) + 0.168
			* IMG_BLUE(x,y));
			rgb[2] = (int)(0.272 * IMG_RED(x,y) + 0.534 * IMG_GREEN(x,y) + 0.131
			 * IMG_BLUE(x,y));
			IMG_RED(x,y) = (rgb[0] > 255) ? 255 : rgb[0];
			IMG_GREEN(x,y) = (rgb[1] > 255) ? 255 : rgb[1];
			IMG_BLUE(x,y) = (rgb[2] > 255) ? 255 : rgb[2];
		}
}

void effect_grayscale(unsigned char *img)
{
	int value;
	int y;
	int x;

	y = -1;
	while (++y < HEIGHT && (x = -1))
		while (++x < WIDTH)
		{
			value = (IMG_BLUE(x,y)
					+ IMG_GREEN(x,y)
					+ IMG_RED(x,y)) / 3;
		IMG_BLUE(x,y) = value;
		IMG_GREEN(x,y) = value;
		IMG_RED(x,y) = value;
		}
}


void kernel_fill(char kernel[9][9], char *s, int n)
{
	char i;
	char j;
	char **buffer;
	char k;

	kernel[0][0] = 1;
	k = 0;
	buffer = ft_strsplitspace(s);
	i = -1;
	j = -1;

	while (++j < n && (i = -1))
		while (++i < n)
			kernel[j][i] = ft_atoi(buffer[k++]);
}

void 		kernel_type(char kernel[9][9], char type, int *n)
{
	if (type == EFFECT_BLUR)
	{
		kernel_fill(kernel, "1 2 1 2 4 2 1 2 1", 3);
	}
	else 	if (type == EFFECT_MOTION_BLUR)
		{
			//kernel_fill(kernel, "1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0	0 0 0 1 0 0 0 0 0	0 0 0 0 1 0 0 0 0	0 0 0 0 0 1 0 0 0	0 0 0 0 0 0 1 0 0	0 0 0 0 0 0 0 1 0	0 0 0 0 0 0 0 0 1", 6);
//kernel_fill(kernel, "-1 -2 -1 0 5 0 1 2 1 ", 3);
kernel_fill(kernel, "1 0 0 0 1 0 0 0 1 ", 3);
						//							*n = 3;

		}
	else if (type == EFFECT_SOBEL)
		kernel_fill(kernel, "-1 -1 -1 -1 8 -1 -1 -1 -1", 3);
	else if (type == EFFECT_SHARPEN1)
		kernel_fill(kernel, "-1 -1 -1 -1 9 -1 -1 -1 -1", 3);
	else if (type == EFFECT_EMBOSS)
		kernel_fill(kernel, "-1 -1 0 -1 0 1 0 1 1", 3);
	else if (type == EFFECT_SHARPEN2)
		kernel_fill(kernel, "1 1 1 1 -7 1 1 1 1", 3);
	// const char kernel[3][3] = { {0, 1, 0}, {1,  -4, 1}, {0, 1, 0}};
	// const char kernel[3][3] = { {0, -1, 0}, {-1,  5, -1}, {0, -1, 0}};
	// const char kernel[3][3] = { {-1, -1, -1}, {-1,  8, -1}, {-1, -1, -1}};
	// const char kernel[3][3] = { {1, 0, -1},	{2,  0, -2}, {1, 0, -1}};
	// const char kernel[3][3] = { {3, 10, 3},	{0,  0, 0},	{-3, -10, -3}};
	// const char kernel[3][3] = { {1, 2, 1}, {2,  1, 2},	{1, 2, 1}};
	// const char kernel[3][3] = { {0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
	// const char kernel[3][3] = { {1, 0, -1},	{0, 0, 0},	{-1, 0, 1}};
	// const char kernel[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	if (*n != 9)
			*n = 3;
}

unsigned int min_max(int n)
{
	 n = n > C ? C : n;
	 n = n < 0 ? 0 : n;
	 return n;
}

void effect_fusion_dm(unsigned char *img, unsigned char *img2)
 {
	 int y;
	 int x;
	 double ratio;
	 double nratio;
	 unsigned char img3[3 * WIDTH * HEIGHT + 50];

	 depth_map(img3);
	 y =-1;
	 while (++y < HEIGHT - 1)
	 {
		 x = -1;
		 while (++x < WIDTH - 1)
		 {
			 	ratio = ((IMG3_BLUE(x, y)) / 122.5);
				nratio = 1 - ratio;
				//printf(" calculation %d ratio %f ",IMG_BLUE(x,y), ratio);
			 	IMG_BLUE(x, y) = min_max((IMG_BLUE(x, y) * ratio) + (IMG2_BLUE(x, y) * nratio));
				IMG_GREEN(x, y) = min_max((IMG_GREEN(x, y) * ratio) + (IMG2_GREEN(x, y) * nratio));
				IMG_RED(x, y) = min_max((IMG_RED(x, y) * ratio) + (IMG2_RED(x, y) * nratio));
		 }
	 }
}

void effect_kernel(char type, unsigned char *img, int div)
{
	int y;
	int x;
	int i;
	int j;
	int rgb[3];
	unsigned char img2[3 * WIDTH * HEIGHT];
	unsigned char img3[3 * WIDTH * HEIGHT];
	char kernel[9][9];
	int n;
	int hk;

	kernel_type(kernel, type, &n);
	ft_memcpy(img2, img, (WIDTH * HEIGHT * 3));

	hk = n / 2;
	y = 0;
	while (++y < HEIGHT - 1)
	{
		x = 0;
		while (++x < WIDTH - 1)
		{
			j = -(hk + 1);
			ft_memset(rgb, 0 , 3);
			rgb[0] = 0;
			rgb[1] = 0;
			rgb[2] = 0;

			while (++j < hk + 1 && (i = -(hk + 1)))
			{
				while (++i < hk + 1)
				{
					rgb[0] += IMG2_RED(x + i, y + j) * kernel[hk + j][hk + i] / div;
					rgb[1] += IMG2_GREEN(x + i, y + j) * kernel[hk + j][hk + i] / div;
					rgb[2] += IMG2_BLUE(x + i, y + j) * kernel[hk + j][hk + i] / div;
					}
				}

				IMG_RED(x,y) = min_max(rgb[0]);
				IMG_GREEN(x,y) = min_max(rgb[1]);
				IMG_BLUE(x,y) = min_max(rgb[2]);

		}
	}
	// effect_fusion_dm(img, img2);
}

void effect_black(unsigned char *img, unsigned char *img2)
{
	int y;
	int x;

	y = -1;
	while (++y < HEIGHT && (x = -1))
		while (++x < WIDTH)
		{
			if (IMG2_RED(x, y) > 10)
				{
					IMG_GREEN(x, y) = 0;
					IMG_RED(x, y) = 0;
					IMG_BLUE(x, y) = 0;
					IMG_GREEN(x + 1, y) = 0;
					IMG_RED(x + 1, y) = 0;
					IMG_BLUE(x + 1, y ) = 0;
				}
		}
}

void effect(t_effects *effect, unsigned char *img) {
	int i;
	unsigned char img2[3 * WIDTH * HEIGHT];

	i = 0;
	ft_memcpy(img2, img, (WIDTH * HEIGHT * 3));
	while (++i < EFFECT_MAX)
	{
		if (effect->grayscale == i)
			effect_grayscale(img);
		else if (effect->blur == i){
			effect_kernel(EFFECT_BLUR, img, 16);
			effect_kernel(EFFECT_BLUR, img, 16);
			effect_kernel(EFFECT_BLUR, img, 16);
			effect_kernel(EFFECT_BLUR, img, 16);
			effect_kernel(EFFECT_BLUR, img, 16);
			effect_kernel(EFFECT_BLUR, img, 16);
		}
		else if (effect->motion_blur == i){
			effect_kernel(EFFECT_MOTION_BLUR, img, 3);
			effect_kernel(EFFECT_MOTION_BLUR, img, 3);
			effect_kernel(EFFECT_MOTION_BLUR, img, 3);
			effect_kernel(EFFECT_MOTION_BLUR, img, 3);
			effect_kernel(EFFECT_MOTION_BLUR, img, 3);
			effect_kernel(EFFECT_MOTION_BLUR, img, 3);
			effect_kernel(EFFECT_MOTION_BLUR, img, 3);
			effect_kernel(EFFECT_MOTION_BLUR, img, 3);
		}
		else if (effect->sepia == i)
				effect_sepia(img);
		else if (effect->sobel == i)
				effect_kernel(EFFECT_SOBEL, img, 9);
		else if (effect->sharpen1 == i)
				effect_kernel(EFFECT_SHARPEN1, img, 1);
		else if (effect->sharpen2 == i)
						effect_kernel(EFFECT_SHARPEN2, img, 1);
		else if (effect->emboss == i)
						effect_kernel(EFFECT_EMBOSS, img, 1);
		else if (effect->toon == i)
					{
						ft_memcpy(img2, img, (WIDTH * HEIGHT * 3));
						effect_grayscale(img2);
						effect_kernel(EFFECT_SOBEL, img2, 1);

						//ft_memcpy(img, img2, (WIDTH * HEIGHT * 3));
						effect_black(img, img2);
						ft_memcpy(img2, img, (WIDTH * HEIGHT * 3));
						effect_kernel(EFFECT_SOBEL, img2, 1);
						effect_black(img, img2);

						//effect_kernel(EFFECT_BLUR, img, 9);

					}
		}
		if (effect->stereo)
		{
			effect->img_stereo = (unsigned char*)ft_memalloc(WIDTH * HEIGHT * 3 * sizeof(unsigned char));
			ft_memcpy(effect->img_stereo, img, (WIDTH * HEIGHT * 3));
			return;
		}
		//effect_fusion_dm(img, img2);

}

void effect_fusion(unsigned char *img, unsigned char *img2)
 {
	 int y;
	 int x;
	 y =-1;
	 while (++y < HEIGHT - 1)
	 {
		 x = -1;
		 while (++x < WIDTH - 1)
		 {

			 	IMG_BLUE(x, y) = min_max(IMG_BLUE(x, y) + IMG2_BLUE(x, y));
				IMG_GREEN(x, y) = min_max(IMG_GREEN(x, y) + IMG2_GREEN(x, y));
				IMG_RED(x, y) = min_max(IMG_RED(x, y) + IMG2_RED(x, y));
		 }
	 }
}


void color_filter(unsigned char img[3 * WIDTH * HEIGHT], unsigned char r, unsigned char g, unsigned char b) {

	int y;
	int x;

	y = -1;
	if (!img)
		return;
	while (++y < HEIGHT && (x = -1))
		while (++x < WIDTH)
		{
			IMG_RED(x,y) = r / 255 * IMG_RED(x,y);
			IMG_GREEN(x,y) = g  / 255 * IMG_GREEN(x,y);
			IMG_BLUE(x,y) = b  / 255 * IMG_BLUE(x,y);
		}
}

void depth_map(unsigned char img[3 * WIDTH * HEIGHT]) {
	int y;
	int x;
	double min = 20000;
	double max = 0;
	double **dm;
	double value;

	dm = singleton_depth_map();
	y = -1;
	while (++y < HEIGHT && (x = -1))
		while (++x < WIDTH)
		{
			if (dm[y][x] < min && dm[y][x])
				min = dm[y][x];
			if (dm[y][x] > max)
				max = dm[y][x];
		}
		//printf("min %f max %f\n", min, max);
		y = -1;

	//	printf("value %f\n", value);
	while (++y < HEIGHT && (x = -1))
		while (++x < WIDTH)
		{
			value = ((dm[y][x] - min) / max) * 255;
			//printf("%f\n", dm[y][x]);
			if (dm[y][x] > 0){
			IMG_GREEN(x, y) = value;
			IMG_RED(x, y) = value;
			IMG_BLUE(x, y) = value;
			}
			else {
				IMG_GREEN(x, y) = 0;
				IMG_RED(x, y) = 0;
				IMG_BLUE(x, y) = 0;
			}
		}
}


char *name_anime(int anim)
{
	char *name;
	char *ret;

	name = ft_itoa(anim);
	ret = ft_memalloc(sizeof(char) * ft_strlen(name) + 8);
	ft_memcpy(ret, name, ft_strlen(name));
	ft_strcat(ret,"out.bmp");
	return ret;
}

t_display  *singleton_display(t_display *e) {
		static char init = 0;
		int i;
		static t_display *ret;
		if (!init)
		{
			ret = ft_memalloc(sizeof(t_display));
			init_display(ret);
			init = 1;
		}
		*e = *ret;
}

void	print_img(t_effects *effects,unsigned char img[3 * WIDTH * HEIGHT], int anim)
{
	t_display e;
	int y;
	int x;
	int color;

	singleton_display(&e);
	//init_display(&e);
	effect(effects, img);
	if (effects->stereo)
	{
			color_filter(img, 0, 255, 255);
			effect_fusion(img, img);
	}
	else if(effects->img_stereo)
		{
			color_filter(effects->img_stereo, 255, 0, 0);
			effect_fusion(img, effects->img_stereo);
			free(effects->img_stereo);
			effects->img_stereo = NULL;
		}
	if (effects->stereo)
		ft_memcpy(img, effects->img_stereo,(WIDTH * HEIGHT * 3));

	y = -1;
	while (++y < HEIGHT && (x = -1))
		while (++x < WIDTH)
		{
			color = (img[(x + y * WIDTH) * 3 + 2])
					+ (img[(x + y * WIDTH) * 3 + 1] * 256)
					+ (img[(x + y * WIDTH) * 3] * 256 * 256);
			draw(&e, x, y, color);
			put_pixel32(e.surface, x, y, color);
		}
	pack(&e);
	if (!effects->stereo)
		SDL_SaveBMP(e.surface, name_anime(anim));
}

void	swapdouble(double *a, double *b)
{
	double c;

	c = *a;
	*a = *b;
	*b = c;
}
