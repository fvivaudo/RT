/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/17 15:40:03 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

void 			cast_func2(t_cc *c, int g)
{
	c->coef = (double*)malloc(sizeof(double) * c->sizerays);
	c->newrays = (t_ray*)malloc(sizeof(t_ray) * c->sizenewray);
	c->newout = (t_gpu_out*)malloc(sizeof(t_gpu_out) * c->sizenewray);
	c->newinforays = (t_sray_info*)malloc(sizeof(t_sray_info) * c->sizenewray);
	c->gbis = 0;
	while(++g < c->sizerays)
	{
		cast_func3(c, g);
		cast_func4(c, g);
		if (c->level < MAX_DEPTH_LEVEL && c->transcoef > 0 &&
			 c->cmat->transparency > 0)
		{
			c->par.r = c->ray;
			c->par.n = &c->normal;
			c->par.cmat = c->cmat;
			c->par.inter_point = &c->inter_point;
			c->par.crefraction = &c->crefraction;
			refract(c->par);
			c->newinforays[c->gbis].coef = c->tmpcoef2;
			c->newinforays[c->gbis].previousrefractionindex = c->crefraction;
			c->newrays[c->gbis++] = *c->ray;
			c->coef[g] -= c->tmpcoef2;
		}
	}
}

void 			cast_func5(t_cc *c, int g)
{
	if (c->coef)
	{
		while(++g < c->sizerays)
			update_img(&c->gtool.gpu_out[g].color,
				c->gtool.gpu_in[g].pixel_id, c->coef[g]);
	}
	else
	{
		g = -1;
		while(++g < c->sizerays)
			update_img(&c->gtool.gpu_out[g].color,
				c->gtool.gpu_in[g].pixel_id, 1);
	}
}

void			*cast_ray_thread(void *e)
{
	t_cc c;

	cast_func0(&c, e);
	while (c.level < MAX_DEPTH_LEVEL)
	{
		cast_func1(&c);
		if (c.sizenewray > 0)
			cast_func2(&c, -1);
		cast_func5(&c, -1);
		++c.level;
		c.sizerays = c.sizenewray;
		free(c.gtool.gpu_in);
		c.gtool.gpu_in = c.newrays;
		free(c.gtool.gpu_out);
		c.gtool.gpu_out = c.newout;
		free(c.inforays);
		c.inforays = c.newinforays;
		free(c.coef);
		if (c.sizerays == 0)
			return (NULL);
	}
	return (NULL);
}
