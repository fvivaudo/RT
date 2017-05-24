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

bool				setorient(char **buffer, int *y, t_obj *obj)
{
	if (!ft_strcmp("ORIENTATION", buffer[*y]))
	{
		if (buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3])
		{
			obj->dir = vectorinit(ft_datoi(buffer[*y + 1]),
			ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
			vectornormalize(&obj->dir);
			*y += 4;
			return (TRUE);
		}
	}
	return (FALSE);
}

bool				setslice(char **buffer, int *y, t_obj *obj)
{
	t_obj *slice;

	if (!ft_strcmp("SLICE", buffer[*y]))
	{
		if (buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3] &&
			buffer[*y + 4] && buffer[*y + 5] && buffer[*y + 6])
		{
			slice = (t_obj*)malloc(sizeof(t_obj));
			slice = init_null();
			slice->pos = vectorinit(ft_datoi(buffer[*y + 1]),
			ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
			slice->dir = vectorinit(ft_datoi(buffer[*y + 4]),
			ft_datoi(buffer[*y + 5]), ft_datoi(buffer[*y + 6]));
			vectornormalize(&slice->dir);
			slice->type_obj = TYPE_SLICE;
			lstaddslice(&obj->nextslice, slice);
			*y += 7;
			return (TRUE);
		}
	}
	return (FALSE);
}

bool				setnegative(char **buffer, int *y, t_obj *parent,
	 t_obj **lstobj)
{
	static t_obj* parentsave = NULL;

	if (parent)
		parentsave = parent;
	if (!ft_strcmp("NEGATIVE", buffer[*y]))
	{
		if (!(ft_strcmp(buffer[*y + 1], "SPHERE")))
			*y += init_sphere(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "CONE")))
			*y += init_cone(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "CYLINDER")))
			*y += init_cyl(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "PLANE")))
			*y += init_plane(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "QUADRIC")))
			*y += init_quadric(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "OBJECT")))
			*y += init_object(lstobj, &(*(buffer + *y + 1)), TRUE);
		(*lstobj)->pos = vectoradd(parentsave->pos, (*lstobj)->pos);
		(*lstobj)->type_obj = TYPE_NEGATIVE;
		(*lstobj)->isneg = TRUE;
		return (TRUE);
	}
	return (FALSE);
}


void 		init_cam_func(t_env *e, t_incam *o)
{
	e->cam.lookat = vectorscale(SCREEN_EYE_DIST, e->cam.vdir);
	o->tmp_vdir = vectorinit(0, 0, 1);
	o->tmp_up = vectorinit(0, -1, 0);
	o->up = vectorinit(0, -1, 0);
	o->u = vectorproduct(e->cam.vdir, o->up);
	o->v = vectorproduct(o->u, e->cam.vdir);
	o->u = vectornormalize(&o->u);
	o->v = vectornormalize(&o->v);
	e->cam.viewplanebottomleftpoint = vectorsub(
	vectorsub(e->cam.lookat, vectorscale(HEIGHT / 2, o->v)),
		vectorscale(WIDTH / 2, o->u));
	e->cam.viewplanebottomleftpoint =
		vectoradd(e->cam.viewplanebottomleftpoint, e->cam.eyepoint);
	e->cam.viewplanebottomleftpoint =
		vectoradd(e->cam.viewplanebottomleftpoint, e->cam.lookat);
	e->cam.xincvector = vectorscale(1 / (double)WIDTH,
	(vectorscale(2 * WIDTH / 2, o->u)));
	e->cam.yincvector = vectorscale(1 / (double)HEIGHT,
	(vectorscale(2 * HEIGHT / 2, o->v)));
}

void		init_cam(t_env *e, char **buffer)
{
	t_incam o;

	if (buffer[1] && buffer[2] && buffer[3])
		e->cam.eyepoint = vectorinit(ft_datoi(buffer[1]),
		ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return;
	if (!ft_strcmp("LOOKAT", buffer[4]) && buffer[5] && buffer[6] && buffer[7])
	{
		e->cam.lookat = vectorinit(ft_datoi(buffer[5]), ft_datoi(buffer[6]),
			ft_datoi(buffer[7]));
		e->cam.vdir = vectorsub(e->cam.lookat, e->cam.eyepoint);
		vectornormalize(&e->cam.vdir);
	}
	else if (!ft_strcmp("ORIENTATION", buffer[4]) && buffer[5] && buffer[6]
		&& buffer[7])
	{
		e->cam.vdir = vectorinit(ft_datoi(buffer[5]), ft_datoi(buffer[6]),
			ft_datoi(buffer[7]));
		vectornormalize(&e->cam.vdir);
	}
	else
		return;
	init_cam_func(e, &o);
}
