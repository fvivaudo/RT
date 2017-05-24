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

int 			init_obj_func2(t_niniobj *u, char **buffer)
{
	if (buffer[u->y + 1])
	{
		u->obj->id = ft_atoi(buffer[u->y + 1]);
		u->y += 2;
	}
	else
		return (u->y);
	return (-1234567);
}
int 			init_obj_func3(t_niniobj *o, char **buffer)
{
	if (buffer[o->y + 1])
	{
		o->obj->rotation = ft_datoi(buffer[o->y + 1]) * M_PI / 180.0;
		o->y += 2;
	}
	return (-1234567);
}

int 			init_object_func1(t_niniobj *o, char **buffer, t_obj **lstobj,
	bool neg)
{
	int (res);

	if (!ft_strcmp("ID", buffer[o->y]) &&
		(res = init_obj_func2(o, buffer)) != -1234567)
		return (res);
	else if (!ft_strcmp("POSITION", buffer[o->y]))
	{
		if (buffer[o->y + 1] && buffer[o->y + 2] && buffer[o->y + 3])
		{
			o->obj->pos = vectorinit(ft_datoi(buffer[o->y + 1]),
			ft_datoi(buffer[o->y + 2]), ft_datoi(buffer[o->y + 3]));
			o->y += 4;
		}
		else
			return (o->y);
	}
	else if (setorient(buffer, &o->y, o->obj));
	else if (setslice(buffer, &o->y, o->obj));
	else if (neg == FALSE && setnegative(buffer, &o->y, o->obj, OHAO));
	else if (neg == TRUE && setnegative(buffer, &o->y, NULL, lstobj));
	else if (!ft_strcmp("ROTATE", buffer[o->y]))
		init_obj_func3(o, buffer);
	else
		++o->y;
	return (-1234567);
}

int			init_object(t_obj **lstobj, char **buffer, bool neg)
{
	t_niniobj o;
	int res;

	o.obj = NULL;
	o.y = 2;
	o.id = 0;
	if (buffer[1])
	{
		o.cursor = *lstobj;
		while (o.cursor)
		{
			if (o.cursor->id == ft_atoi(buffer[1]))
				o.obj = o.cursor;
			o.cursor = o.cursor->nextitem;
		}
	}
	if (buffer == NULL)
		return (1);
	while (buffer[o.y] != NULL)
	if ((res = init_object_func1(&o, buffer, lstobj, neg)) != -1234567)
		return (res);
	extractobj(lstobj, o.obj, o.id);
	return (o.y);
}

void 	init_light_func(char **buffer, int y, t_light *light)
{
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("INTENSITY", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3])
			{
				light->intensity = colorinit(ft_datoi(buffer[y + 1]),
				ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				y += 4;
			}
			else
				return;
		}
		else
			++y;
	}
}
