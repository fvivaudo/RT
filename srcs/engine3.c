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

void			get_img_pos(int *x, int *y, int inter)
{
	*x += inter;
	while (*x > WIDTH)
	{
		*x -= WIDTH;
		++(*y);
		if (*y >= HEIGHT)
			*x = WIDTH;
	}
}

t_obj 	*copyallobj_func(t_obj *obj)
{
	t_obj *copy;

	copy = (t_obj*)malloc(sizeof(t_obj));
	copy->id = obj->id;
	copy->type = obj->type;
	copy->material = obj->material;
	copy->pos = obj->pos;
	copy->dir = obj->dir;
	copy->rad = obj->rad;
	copy->rad2 = obj->rad2;
	copy->height = obj->height;
	copy->alpha = obj->alpha;
	copy->quad = obj->quad;
	copy->isneg = obj->isneg;
	copy->rotation = obj->rotation;
	copy->type_obj = obj->type_obj;
	return (copy);
}

t_obj	*copyallobj(t_obj *obj)
{
	t_obj *copy;

	copy = copyallobj_func(obj);
	if (obj->nextneg)
		copy->nextneg = copyallobj(obj->nextneg);
	else
		copy->nextneg = NULL;
	if (obj->nextslice)
		copy->nextslice = copyallobj(obj->nextslice);
	else
		copy->nextslice = NULL;
	if (obj->nextitem)
		copy->nextitem = copyallobj(obj->nextitem);
	else
		copy->nextitem = NULL;
	return (copy);
}

void		effect(t_env *new, t_env *e)
{
	double	tmpred;
	double	tmpgreen;
	double	tmpblue;

	tmpred = new->col.red;
	tmpgreen = new->col.green;
	tmpblue = new->col.blue;
	new->col.red = (tmpred + tmpgreen + tmpblue) * e->ered / 100;
	new->col.green = (tmpred + tmpgreen + tmpblue) * e->egreen / 100;
	new->col.blue = (tmpred + tmpgreen + tmpblue) * e->eblue / 100;
}


t_light	*copyalllights(t_light *light)
{
	t_light *copy;

	copy = (t_light*)malloc(sizeof(t_light));

	copy->pos = light->pos;
	copy->intensity = light->intensity;

	if (light->next)
	{
		copy->next = copyalllights(light->next);
	}
	return (copy);
}
