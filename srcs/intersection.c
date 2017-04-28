/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:58:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:57:12 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

double	getdegree(double rad)
{
	return (rad * 180 / M_PI);
}

t_obj	*intersection(t_env *e, t_ray *r, int id_ignore)
{
	t_obj			*cursor;
	t_obj			*res;
	cursor = e->obj;
	res = NULL;
	double t = MAX_RANGE;
	//e->t is the current minimal distance to an object encountered

	while (cursor)
	{
		if (cursor->id == id_ignore)
		{
		//	cursor = cursor->nextitem;
		//	continue;
		}
		if ((cursor->type == TYPE_SPHERE && iraysphere(r, cursor, &t)) ||
			(cursor->type == TYPE_PLANE && irayplane(r, cursor, &t)) ||
			(cursor->type == TYPE_CYLINDER && iraycylinder(r, cursor, &t)) ||
			(cursor->type == TYPE_CONE && iraycone(r, cursor, &t)) ||
			(cursor->type == TYPE_QUADRIC && irayquadric(r, cursor, &t)))
		{
			e->t = t;
			res = cursor;
		}
		cursor = cursor->nextitem;
	}
	return (res);
	// if an intersection was found
}

t_obj	*computeray(t_env *e)
{
	t_obj *res;

	if ((res = intersection(e, &e->r, -1)))
	{
		e->id = res->id;
		//e->scaled = distance between start of ray and object intersection
		e->scaled = vectorscale(e->t, e->r.dir);
		//e->newstart = object intersection
		e->newstart = vectoradd(e->r.start, e->scaled);
		if (res->normobj.set == TRUE) //set it somewhere to 0 somewhere
		{
			if (res->normobj.specificnormal == TYPE_SPHERE)
				normalsphere(e, (t_obj*)&res->normobj);//add reversen somewhere
			else if (res->normobj.specificnormal == TYPE_PLANE)
				normalplane(e, (t_obj*)&res->normobj);
			else if (res->normobj.specificnormal == TYPE_CYLINDER)
				normalcylinder(e, (t_obj*)&res->normobj);
			else if (res->normobj.specificnormal == TYPE_CONE)
				normalcone(e, (t_obj*)&res->normobj);
			else if (res->normobj.specificnormal == TYPE_QUADRIC)
				normalquadric(e, (t_obj*)&res->normobj);
		}
		else
		{
			if (res->type == TYPE_SPHERE)
				normalsphere(e, res);//add reversen somewhere
			else if (res->type == TYPE_PLANE)
				normalplane(e, res);
			else if (res->type == TYPE_CYLINDER)
				normalcylinder(e, res);
			else if (res->type == TYPE_CONE)
				normalcone(e, res);
			else if (res->type == TYPE_QUADRIC)
				normalquadric(e, res);
		}
	}
	return (res);
}
