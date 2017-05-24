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

void 			reflect_and_refract_func(t_raus *o, t_env *e)
{
	e->cmat = o->collide->material;
	o->angle = cos(vectordot(e->r.dir, vectorscale(1.0, e->n)));
	o->R0 = pow((e->crefraction - e->cmat.refraction) / (e->crefraction +
		e->cmat.refraction), 2);
	e->reflecoef = o->R0 + (1.0 - o->R0) * pow((1.0 - o->angle), 5) * (1.0 -
		e->cmat.transparency);
	e->transcoef = 1.0 - e->reflecoef;
	o->originalcoef = e->coef;
	o->tmpcoef1 = e->coef * e->reflecoef;
	o->tmpcoef2 = e->coef * e->transcoef;
	o->refracolor = colorinit(0, 0, 0);
	o->reflecolor = colorinit(0, 0, 0);
	++e->level;
}
void 			reflect_and_refract_func2(t_raus *o, t_env *e)
{
	o->res.red = o->reflecolor.red * e->reflecoef + o->refracolor.red *
		(1 - e->reflecoef) + e->col.red;
	o->res.green = o->reflecolor.green * e->reflecoef + o->refracolor.green
		* (1 - e->reflecoef) + e->col.green;
	o->res.blue = o->reflecolor.blue * e->reflecoef + o->refracolor.blue
		* (1 - e->reflecoef) + e->col.blue;
}

void 			reflect_and_refract_func3(t_raus *o, t_env *e)
{
	e->coef = o->tmpcoef1;
	o->tmp = vectorscale(2 * vectordot(e->r.dir, e->n), e->n);
	e->r.dir = vectorsub(e->r.dir, o->tmp);
	e->r.start = vectoradd(e->newstart, e->r.dir);
}

t_color			reflect_and_refract(t_env e)
{
	t_raus o;

	e.t = MAX_RANGE;
	o.collide = computeray(&e);
	if (!o.collide)
	{
		o.res = colorinit(0, 0, 0);
		return (o.res);
	}
	reflect_and_refract_func(&o, &e);
	if (e.level < MAX_DEPTH_LEVEL && e.cmat.reflection > 0)
	{
		reflect_and_refract_func3(&o, &e);
		vectornormalize(&e.r.dir);
		o.reflecolor = reflect_and_refract(e);
	}
	if (e.level < MAX_DEPTH_LEVEL && e.transcoef > 0)
	{
		e.coef = o.tmpcoef2;
		o.refracolor = reflect_and_refract(e);
	}
	if (e.cmat.transparency < 1.0 && e.cmat.reflection < 1.0)
		deal_shadow(&e);
	reflect_and_refract_func2(&o, &e);
	return (o.res);
}

unsigned char	*update_img(t_color *col, int pixel_id, double coef)
{
	static unsigned char	img[3 * WIDTH * HEIGHT];

	if (pixel_id > WIDTH * HEIGHT || pixel_id < 0)
		return (img);
	if (col)
	{
		col->red = col->red > C ? C : col->red;
		img[pixel_id * 3 + 0] += (unsigned char)(col->red * C * coef);
		col->green = col->green > C ? C : col->green;
		img[pixel_id * 3 + 1] += (unsigned char)(col->green * C * coef);
		col->blue = col->blue > C ? C : col->blue;
		img[pixel_id * 3 + 2] += (unsigned char)(col->blue * C * coef);
	}

	return (img);
}
