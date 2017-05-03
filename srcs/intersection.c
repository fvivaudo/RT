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

t_objgpu	*intersection(t_env *e, t_ray *r)
{
	int 				i;
	t_objgpu			*res;

	i = 0;
	res = NULL;
	double t = MAX_RANGE;
	//e->t is the current minimal distance to an object encountered

	while (e->objgpu[i].set == TRUE)
	{
		if ((e->objgpu[i].type == TYPE_SPHERE && iraysphere(r, &e->objgpu[i], &t)) ||
			(e->objgpu[i].type == TYPE_PLANE && irayplane(r, &e->objgpu[i], &t)) ||
			(e->objgpu[i].type == TYPE_CYLINDER && iraycylinder(r, &e->objgpu[i], &t)) ||
			(e->objgpu[i].type == TYPE_CONE && iraycone(r, &e->objgpu[i], &t)) ||
			(e->objgpu[i].type == TYPE_QUADRIC && irayquadric(r, &e->objgpu[i], &t)))
		{
			e->t = t;
			res = &e->objgpu[i];
		}
		++i;
	}
	return (res);
	// if an intersection was found
}

t_objgpu	*computeray(t_env *e)
{
	t_objgpu *res;

	if ((res = intersection(e, &e->r)))
	{
		e->id = res->id;
		//e->scaled = distance between start of ray and object intersection
		e->scaled = vectorscale(e->t, e->r.dir);
		//e->newstart = object intersection
		e->newstart = vectoradd(e->r.start, e->scaled);
		if (res->normobj.set == TRUE) //set it somewhere to 0 somewhere
		{
			if (res->normobj.specificnormal == TYPE_SPHERE)
				normalsphere(e, (t_objgpu*)&res->normobj);//add reversen somewhere
			else if (res->normobj.specificnormal == TYPE_PLANE)
				normalplane(e, (t_objgpu*)&res->normobj);
			else if (res->normobj.specificnormal == TYPE_CYLINDER)
				normalcylinder(e, (t_objgpu*)&res->normobj);
			else if (res->normobj.specificnormal == TYPE_CONE)
				normalcone(e, (t_objgpu*)&res->normobj);
			else if (res->normobj.specificnormal == TYPE_QUADRIC)
				normalquadric(e, (t_objgpu*)&res->normobj);
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
