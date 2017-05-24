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

int		init_cone(t_obj **lstobj, char **buffer, bool neg)
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
		if ((res = init_cone_func1(&u, buffer, lstobj, neg)) != -1234567)
			return (res);
	u.obj->type = TYPE_CONE;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}

int 			init_sphere_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
	bool neg)
{
	int res;

	if (!ft_strcmp("ID", buffer[u->y]) &&
		(res = init_cyl_func2(u, buffer)) != -1234567)
		return (res);
	else if (setmat(buffer, &u->y, &u->material));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y, u->obj,
		&u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else if (!ft_strcmp("RADIUS", buffer[u->y]))
	{
		if (buffer[u->y + 1])
		{
			u->obj->rad = ft_datoi(buffer[u->y + 1]);
			u->y += 2;
		}
		else
			return (u->y);
	}
	else
		++u->y;
	return (-1234567);
}

int 			init_plane_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
	bool neg)
{
	if (!ft_strcmp("ID", buffer[u->y]))
	{
		if (buffer[u->y + 1])
		{
			u->obj->id = ft_atoi(buffer[u->y + 1]);
			u->y += 2;
		}
		else
			return (u->y);
	}
	else if (setmat(buffer, &u->y, &u->material));
	else if (setorient(buffer, &u->y, u->obj));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y, u->obj,
		 &u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else
		++u->y;
	return (-1234567);
}

int		init_sphere(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	u.obj = init_null();
	u.y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u.obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]),
		ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[u.y] != NULL)
	if ((res = init_sphere_func1(&u, buffer, lstobj, neg)) != -1234567)
		return (res);
	u.obj->type = TYPE_SPHERE;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}

int		init_plane(t_obj **lstobj, char **buffer, bool neg)
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
	if ((res = init_plane_func1(&u, buffer, lstobj, neg)) != -1234567)
		return (res);
	u.obj->type = TYPE_PLANE;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}
