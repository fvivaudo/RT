/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:57:51 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/17 16:34:23 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* init objets + lights */

#include <rtv1.h>
#include <stdio.h>
#include <math.h>

void	init_light(t_env *e, char **buffer)
{
	int			y;
	t_light		*light;

	light = (t_light*)malloc(sizeof(t_light));
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		light->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]),
			ft_datoi(buffer[3]));
	else
		return;
	init_light_func(buffer, y, light);
	light->next = NULL;
	lstaddlight(&e->lights, light);
}

t_obj	*copyobj(t_obj *obj)
{
	t_obj *copy;

	copy = (t_obj*)malloc(sizeof(t_obj));
	ft_memcpy(copy, obj, sizeof(t_obj));
	if (obj->nextneg)
		copy->nextneg = copyobj(obj->nextneg);
	else
		copy->nextneg = NULL;
	if (obj->nextslice)
		copy->nextslice = copyobj(obj->nextslice);
	else
		copy->nextslice = NULL;
	if (obj->nextitem)
		copy->nextitem = copyobj(obj->nextitem);
	else
		copy->nextitem = NULL;
	return (copy);
}

void 		rotateinnercomponents(t_obj *obj, t_obj *child)
{
	t_obj *cursor;

	cursor = child->nextneg;
	while (cursor)
	{
		cursor->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir,
			vectoradd(cursor->dir, cursor->pos), obj->rotation);
		cursor->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir,
			vectoradd(obj->pos, cursor->pos), obj->rotation);
		cursor->dir = vectorsub(cursor->dir, cursor->pos);
		cursor = cursor->nextitem;
	}
	cursor = child->nextslice;
	while (cursor)
	{
		cursor->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir,
			vectoradd(cursor->dir, cursor->pos), obj->rotation);
		cursor->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir,
			vectoradd(obj->pos, cursor->pos), obj->rotation);
		cursor->dir = vectorsub(cursor->dir, cursor->pos);
		cursor->rotation = 0;
		cursor = cursor->nextslice;
	}
}

void 		extract_obj_func(t_extobj *o, int id, t_obj *obj)
{
	o->tmp = copyobj(o->cursor);
	o->tmp->id = id;
	o->worldfd = vectorinit(0, 0, -1);
	vectornormalize(&o->rot_axis);
	o->tmp->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir,
		vectoradd(o->tmp->dir, o->tmp->pos), obj->rotation);
	o->tmp->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir,
		vectoradd(obj->pos, o->tmp->pos), obj->rotation);
	o->tmp->dir = vectorsub(o->tmp->dir, o->tmp->pos);
	if (obj->rotation && (o->tmp->nextneg || o->tmp->nextslice))
		rotateinnercomponents(obj, o->tmp);
	vectornormalize(&o->tmp->dir);
	o->rot_angle = acos(vectordot(o->worldfd, obj->dir));
	o->rot_axis = vectorproduct(o->worldfd, obj->dir);
	if (o->rot_axis.x || o->rot_axis.y || o->rot_axis.z)
	{
		o->tmp->pos = vectorrotate(o->tmp->pos, o->rot_axis, o->rot_angle);
		o->tmp->pos = vectoradd(o->tmp->pos, obj->pos);
	}
	else
		o->tmp->pos = vectoradd(o->tmp->pos, obj->pos);
}

void		extractobj(t_obj **lstobj, t_obj *obj, int id)
{
	t_extobj o;

	o.cursor = obj->nextchild;
	while (o.cursor)
	{
		if (o.cursor->nextchild)
		{
			if (o.cursor->type == 0)
				o.cursor->pos = vectoradd(o.cursor->pos, obj->pos);
			extractobj(lstobj, o.cursor, id);
		}
		else if (o.cursor->type > 0)
		{
			extract_obj_func(&o, id, obj);
			lstaddobj(lstobj, o.tmp);
		}
		o.cursor = o.cursor->nextitem;
	}
}
