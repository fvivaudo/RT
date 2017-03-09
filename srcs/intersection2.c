/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbompoil <mbompoil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:01 by fvivaudo          #+#    #+#             */
/*   Updated: 2016/12/16 12:15:58 by mbompoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

//rotate slice

//returns a distance
int		irayslice(t_ray *r, t_obj *obj, double *dist)
{
	t_vec	tmp;
	double	tmpt;
	t_obj	*cursor;
	double	dot;

	t_vec relativepos; //position of intersection
	t_vec relativedir;
//	t_vec sliceinter;

	cursor = obj->nextslice;
//	printf("slice->pos.x = %g, slice->pos.y = %g, slice->pos.z = %g\n", slice->pos.x, slice->pos.y, slice->pos.z);
	if (obj->t[0] < 0 && obj->t[1] < 0)
	{
		return (FALSE);
	}
/*	if (obj->t[0] > obj->t[1])
	{
		tmptswitch = obj->t[0];
		obj->t[0] = obj->t[1];
		obj->t[1] = tmptswitch;
	}*/

	if (obj->t[0] > 0)
		relativepos = vectoradd(r->start, vectorscale(obj->t[0], r->dir)); //tmpstart instead?
	else
		relativepos = vectoradd(r->start, vectorscale(obj->t[1], r->dir)); //tmpstart instead?
	relativepos = vectorsub(obj->pos, relativepos); //now in local space?
	relativedir = /*vectorsub(cursor->pos, */relativepos/*)*/; //from local center position to intersection
	vectornormalize(&relativedir);
//relative dir should be a vector going from the slice toward emptiness/sliced part

	while (cursor)
	{
//case 0 =  unreachable slice, 2 cases, touching object or touching void
//case 1 =  intersection with plan, facing plan
//case 2 =  intersection with plan, not facing plan

//case 3 =  no intersection with plan and good dir
//case 3 =  no intersection with plan and wrong dir
//		printf("cursor->dir.x = %g cursor->dir.y = %g cursor->dir.z = %g\n", cursor->dir.x, cursor->dir.y, cursor->dir.z);
		tmp = vectorsub(vectoradd(cursor->pos, obj->pos), r->start);
		dot = vectordot(cursor->dir, r->dir);
		if (dot)
		{
		//ft_putendl("ok0");
			tmpt = vectordot(cursor->dir, tmp) / dot;

//			sliceinter = vectoradd(r->start, vectorscale(tmpt, r->dir));
//			relativedir = vectorsub(relativepos, sliceinter); //from slice intersection toward sphere intersection
//	printf("relativedir.x = %g relativedir.y = %g relativedir.z = %g\n", relativedir.x, relativedir.y, relativedir.z);
//			vectornormalize(&relativedir);
//	printf("relativepos.x = %g relativepos.y = %g relativepos.z = %g\n", relativepos.x, relativepos.y, relativepos.z);
//	printf("relativedir.x = %g relativedir.y = %g relativedir.z = %g\n", relativedir.x, relativedir.y, relativedir.z);
			//printf("tmpt = %g\nt0 = %g\n obj->t[1] = %g, dot = %g\n", tmpt, obj->t[0], obj->t[1], dot);
			//check if slice is inside the object
			//if not, maybe we're hitting nothing, or we're hitting the object
			if ((obj->t[0] > tmpt && obj->t[1] > tmpt) || (obj->t[0] < tmpt && obj->t[1] < tmpt))
			{
				//obj->t[0] will be the closest point
				/*if ((obj->t[0] > obj->t[1] && obj->t[1] > 0) || (obj->t[0] < 0 && obj->t[1] > 0))
				{
					tmptswitch = obj->t[0];
					obj->t[0] = obj->t[1];
				}*/
			//	else
			//	{
			//		return (FALSE); //no conceivable distance, i think
			//	}


				//object is fully in front of slice and slice is aligned on r->dir
				if (obj->t[1] < tmpt && dot > 0)
				{
//		ft_putendl("ok2");
					if (obj->t[0] > 0 && obj->t[0] <= *dist)
					{
						*dist = obj->t[0];
					}
					else if (obj->t[1] > 0 && obj->t[1] <= *dist)
					{
						*dist = obj->t[1];
					}
				//	return (TRUE);
				}//object is fully behind slice and slice is directed toward r->start
				else if (obj->t[0] > tmpt && dot < 0)
				{
					if (obj->t[0] > 0 && obj->t[0] <= *dist)
					{
		//ft_putendl("ok34");
						*dist = obj->t[0];
					}
					else if (obj->t[1] > 0 && obj->t[1] <= *dist)
					{
		//ft_putendl("ok35");
						*dist = obj->t[1];
					}
				}
				else// if (obj->t[0] != *dist && obj->t[1] != *dist)
				{
				//	ft_putendl("ok4");                                                                                                            
				//	*dist = tmptswitch;
				//	return (TRUE);
					return (FALSE);
				}
				//	return (TRUE); //slice is out of object
			}
			else if (dot > 0 /*vectordot(relativedir, cursor->dir) > 0*/) // ray coming toward sliced surface
			{
				//obj->t[0] will be the farthest point

				//ft_putendl("ok4");
				obj->reversen = TRUE;
				if (obj->t[1] > 0)
				{
					*dist = obj->t[1];
				}
				else if (obj->t[0] > 0)
				{
					*dist = obj->t[0];
				}
				//return (TRUE);
				//				*dist = tmpt;
				//obj->type = TYPE_PLANE;
			}
			else if (dot < 0 /*vectordot(relativedir, cursor->dir) < 0*/) // ray coming toward object untouched surface
			{
		//ft_putendl("ok5");
				//obj->t[0] will be the closest point
				if (obj->t[0] > 0)
				{
					*dist = obj->t[0];
				}
				else if (obj->t[1] > 0)
				{
					*dist = obj->t[1];
				}
				//return (TRUE);
				//obj->type = TYPE_PLANE;
			}
		}
		else
		{
//			ft_putendl("ok6");
//			return (FALSE);
		}
		//ft_putendl("ok7");
		cursor = cursor->nextslice;
	}
	//	ft_putendl("ok6");
				//	*dist = t0;
	return (TRUE);
}

int		irayneg(t_ray *r, t_obj *obj, double *dist, t_env *e)
{
	t_obj	*cursor;
	double 	tmax;
	double	current;

//	t_vec sliceinter;

			//	ft_putendl("unhandled case irayneg");
	cursor = obj->nextneg;
//	printf("slice->pos.x = %g, slice->pos.y = %g, slice->pos.z = %g\n", slice->pos.x, slice->pos.y, slice->pos.z);
	if (obj->t[0] < 0 && obj->t[1] < 0)
	{
		return (FALSE);
	}
	current = obj->t[0];
	while (cursor)
	{
//			printf("1cursor->t[0] = %g, cursor->t[1] = %g, obj->t[0] = %g, obj->t[1] = %g\n", cursor->t[0], cursor->t[1], obj->t[0], obj->t[1]);
		tmax = MAX_RANGE;
		if ((cursor->type == TYPE_SPHERE && iraysphere(r, cursor, &tmax, e)) ||
			(cursor->type == TYPE_PLANE && irayplane(r, cursor, &tmax, e)) ||
			(cursor->type == TYPE_CYLINDER && iraycylinder(r, cursor, &tmax, e)) ||
			(cursor->type == TYPE_CONE && iraycone(r, cursor, &tmax, e)) ||
			(cursor->type == TYPE_QUADRIC && irayquadric(r, cursor, &tmax, e)))
		{
			//case 1 hole going through object
			//case 2 hole in front object
			//case 3 hole behind object
			if (cursor->t[0] < obj->t[0] && cursor->t[1] > obj->t[1])
			{
				printf("case 1 = cursor->t[0] = %g, cursor->t[1] = %g, obj->t[0] = %g, obj->t[1] = %g\n", cursor->t[0], cursor->t[1], obj->t[0], obj->t[1]);
				return (FALSE);
			}
			else if (cursor->t[0] < obj->t[0] && cursor->t[1] < obj->t[1])
			{
		//		ft_putendl("case 2");
				printf("case 2 = cursor->t[0] = %g, cursor->t[1] = %g, obj->t[0] = %g, obj->t[1] = %g\n", cursor->t[0], cursor->t[1], obj->t[0], obj->t[1]);
				if (cursor->t[1] > current)
				{
					obj->normobj = cursor;
					obj->normal = cursor->normal;
					current = cursor->t[1]; //we need the deepest hole
				}
			}
		}
		//it should be nextitem and not nextneg
		cursor = cursor->nextitem;
	}
	//	ft_putendl("ok6");
				//	*dist = t0;
	*dist = current;
	return (TRUE);
}

//negative object model working with shadow
//a negative object going through an object (t0 and t1 of negative larger than both t0 and t1 of native object)
int		iraysphere(t_ray *r, t_obj *obj, double *t0, t_env *e)
{
	double	abcdiscr[5];
	double	t[2];
	t_vec	dist;
	bool	ret;

	ret = FALSE;
	//double tmpt;

	abcdiscr[0] = vectordot(r->dir, r->dir);
	dist = vectorsub(r->start, obj->pos);
	abcdiscr[1] = 2 * vectordot(r->dir, dist);
	abcdiscr[2] = vectordot(dist, dist) - (obj->rad * obj->rad);
	abcdiscr[3] = abcdiscr[1] * abcdiscr[1] - 4 * abcdiscr[0] * abcdiscr[2];

	if (abcdiscr[3] < 0)
	{
		return (FALSE);
	}
	else
	{
		abcdiscr[4] = sqrtf(abcdiscr[3]);
		t[0] = (-(abcdiscr[1]) + abcdiscr[4]) / (2);
		t[1] = (-(abcdiscr[1]) - abcdiscr[4]) / (2);

		if (t[0] < t[1])
		{
			obj->t[0] = t[0];
			obj->t[1] = t[1];
		}
		else
		{
			obj->t[0] = t[1];
			obj->t[1] = t[0];
		}

		if (t[0] < 0 && t[1] < 0) // this is not necesarily true, if i have a negative object, it can extend T and bring it into view
		{
			return(FALSE);
		}

		obj->normal = normalsphere;

		//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
		if (obj->nextslice)
		{
			ret = irayslice(r, obj, t0);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg)
		{
			ret = irayneg(r, obj, t0, e);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}

		if (/*(t[0] > 0.001f) && */(t[0] < *t0))
		{
		//printf("tfinal = %g\n", t[0]);

			*t0 = t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}
