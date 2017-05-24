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

void 			init_quadric_func2(t_init_cyl *u, char **buffer)
{
	u->quad.a = ft_datoi(buffer[u->y + 1]);
	u->quad.b = ft_datoi(buffer[u->y + 2]);
	u->quad.c = ft_datoi(buffer[u->y + 3]);
	u->quad.f = ft_datoi(buffer[u->y + 4]);
	u->quad.g = ft_datoi(buffer[u->y + 5]);
	u->quad.h = ft_datoi(buffer[u->y + 6]);
	u->quad.p = ft_datoi(buffer[u->y + 7]);
	u->quad.q = ft_datoi(buffer[u->y + 8]);
	u->quad.r = ft_datoi(buffer[u->y + 9]);
	u->quad.d = ft_datoi(buffer[u->y + 10]);
	u->y += 11;
}

int 			init_quadric_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
	bool neg)
{
	int res;

	if (!ft_strcmp("ID", buffer[u->y]) &&
		(res = init_cyl_func2(u, buffer)) != -1234567)
		return (res);
	else if (setmat(buffer, &u->y, &u->material));
	else if (setorient(buffer, &u->y, u->obj));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y, u->obj,
		&u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else if (!ft_strcmp("PARAM", buffer[u->y]))
	{
		if (buffer[u->y + 1] && buffer[u->y + 2] && buffer[u->y + 3]
			&& buffer[u->y + 4] && buffer[u->y + 5] && buffer[u->y + 6]
			&& buffer[u->y + 7] && buffer[u->y + 8] && buffer[u->y + 9]
			&& buffer[u->y + 10])
			init_quadric_func2(u, buffer);
		else
			return (u->y);
	}
	else
		++u->y;
	return (-1234567);
}
int		init_quadric(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	u.obj = init_null();
	u.y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u.obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]),
		ft_datoi(buffer[3]));
	else
		return (0);
	while (buffer[u.y] != NULL)
	if ((res = init_quadric_func1(&u, buffer, lstobj, neg)) != -1234567)
		return (res);
	double	rot_angle;
	t_vec	rot_axis;
	t_vec	y_axis;
	y_axis = vectorinit(0, 1, 0);
	rot_angle = acos(vectordot(y_axis, u.obj->dir));
	rot_axis = vectorproduct(y_axis, u.obj->dir);
	if (rot_angle)
		u.obj->quad = quadricrotate(u.quad, rot_axis, rot_angle, u.obj->pos);
	else
		u.obj->quad = quadricrotate(u.quad, y_axis, 0, u.obj->pos);
	u.obj->type = TYPE_QUADRIC;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}

int 			init_torus_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
	bool neg)
{
	int res;

	if (!ft_strcmp("ID", buffer[u->y]) &&
		(res = init_cyl_func2(u, buffer)) != -1234567)
		return (res);
	else if (setmat(buffer, &u->y, &u->material));
	else if (setorient(buffer, &u->y, u->obj));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y,
		u->obj, &u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else if (!ft_strcmp("RADIUS", buffer[u->y]))
	{
		if (buffer[u->y + 1] && buffer[u->y + 2])
		{
			u->obj->rad = ft_datoi(buffer[u->y + 1]);
			u->obj->rad2 = ft_datoi(buffer[u->y + 2]);
			u->y += 3;
		}
		else
			return (u->y);
	}
	else
		++u->y;
	return (-1234567);
}

int					init_torus(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	u.obj = init_null();
	u.y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u.obj->pos = vectorinit(ft_datoi(buffer[1]),
		ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[u.y] != NULL)
	if ((res = init_torus_func1(&u, buffer, lstobj, neg)) != -1234567)
		return (res);
	u.obj->type = TYPE_TORUS;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}
