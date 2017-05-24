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

t_objcomplement *init_comp(t_objcomplement *comp)
{
	comp->reversen = FALSE;
	comp->normal = NULL;
	comp->normobj = NULL;
	return (comp);
}

t_obj	*intersection(t_env *e, t_ray *r, t_objcomplement *comp)
{
	t_obj			*cursor;
	t_obj			*res;
	t_objcomplement tmpcomp;
	double t;

	cursor = e->obj;
	res = NULL;
	t = MAX_RANGE;
	while (cursor)
	{
		if ((CU == TYPE_SPHERE && iraysphere(r, CO, &t, IC)) ||
			(CU == TYPE_PLANE && irayplane(r, CO, &t, IC)) ||
			(CU == TYPE_CYLINDER && iraycylinder(r, CO, &t, IC)) ||
			(CU == TYPE_CONE && iraycone(r, CO, &t, IC)) ||
			(CU == TYPE_QUADRIC && irayquadric(r, CO, &t, IC)) ||
			 (CU == TYPE_TORUS && iraytorus(r, CO, &t, IC)))
		{
			*comp = tmpcomp;
			e->t = t;
			res = cursor;
		}
		cursor = cursor->nextitem;
	}
	return (res);
}

t_obj	*computeray(t_env *e)
{
	t_obj 			*res;
	t_objcomplement comp;

	comp.reversen = FALSE;
	comp.normal = NULL;
	comp.normobj = NULL;
	if ((res = intersection(e, &e->r, &comp)))
	{
		e->id = res->id;
		e->scaled = vectorscale(e->t, e->r.dir);
		e->newstart = vectoradd(e->r.start, e->scaled);
		if (comp.normobj)
			comp.normal(e, comp.normobj, &comp);
		else
			comp.normal(e, res, &comp);
	}
	return (res);
}
