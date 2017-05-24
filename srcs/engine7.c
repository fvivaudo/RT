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

void 			cast_func0(t_cc *c, t_env *e)
{
	c->interval = ((t_thread_task*)e)->i;
	c->env = ((t_thread_task*)e)->arg;
	c->new = cast_init_new(c->env);
	get_img_pos(&c->new->x, &c->new->y, c->interval);
	c->gtool = init_gtool(c->env);
	cast_ray_thread_func(c->new, &c->gtool, &c->interval);
	c->level = 0;
	c->sizerays = HEIGHT * WIDTH;
	c->env->col = colorinit(0, 0, 0);
	init_opencl(&c->gtool, &c->cl);
	c->inforays = NULL;
}


void 			cast_func1(t_cc *c)
{
	int g;

	g = -1;
	c->newout = NULL;
	load_opencl(&c->gtool, &c->cl, c->sizerays);
	c->sizenewray = 0;
	while (++g < c->sizerays)
	{
		if (c->gtool.gpu_in_objs[c->gtool.gpu_out[g].id].material.transparency)
			++c->sizenewray;
		if (c->gtool.gpu_in_objs[c->gtool.gpu_out[g].id].material.reflection)
			++c->sizenewray;
	}
	c->coef = NULL;
}

void 			cast_func3(t_cc *c, int g)
{
	c->normal = c->gtool.gpu_out[g].normal;
	c->ray = &c->gtool.gpu_out[g].r;
	c->cmat = &c->gtool.gpu_in_objs[c->gtool.gpu_out[g].id].material;
	c->inter_point = vectoradd(c->ray->start,
		vectorscale(c->gtool.gpu_out[g].t, c->ray->dir));
	if (c->level)
	{
		c->coef[g] = c->inforays[g].coef;
		c->crefraction = c->inforays[g].previousrefractionindex;
	}
	else
	{
		c->coef[g] = 1;
		c->crefraction = 1;
	}
}

void 			cast_func6(t_cc *c, int g)
{
	c->angle = cos(vectordot(c->ray->dir, vectorscale(1.0,
		c->gtool.gpu_out[g].normal)));
	c->R0 = pow((c->crefraction - c->cmat->refraction) / (c->crefraction
		+ c->cmat->refraction), 2);
	c->reflecoef = c->R0 + (1.0 - c->R0) * pow((1.0 - c->angle), 5) * (1.0 -
		c->cmat->transparency);
	c->transcoef = 1.0 - c->reflecoef;
	c->tmpcoef1 = c->coef[g] * c->reflecoef;
	c->tmpcoef2 = c->coef[g] * c->transcoef;
	c->refracolor = colorinit(0, 0, 0);
	c->reflecolor = colorinit(0, 0, 0);
}

void 			cast_func4(t_cc *c, int g)
{
	cast_func6(c, g);
	if (c->level < MAX_DEPTH_LEVEL && c->cmat->reflection > 0)
	{
		c->tmp = vectorscale(2 * vectordot(c->ray->dir,
			c->gtool.gpu_out[g].normal), c->gtool.gpu_out[g].normal);
		c->ray->dir = vectorsub(c->ray->dir, c->tmp);
		c->ray->start = vectoradd(c->inter_point, c->ray->dir);
		vectornormalize(&c->ray->dir);
		c->newinforays[c->gbis].coef = c->tmpcoef1;
		c->newinforays[c->gbis].previousrefractionindex = 1;
		c->newrays[c->gbis++] = *c->ray;
		c->coef[g] -= c->tmpcoef1;
	}
}
