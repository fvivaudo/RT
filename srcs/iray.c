/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iray.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:01 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:57:02 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

int		iraycone2(double abcd[4], double t[2], double *t0)
{
	if (abcd[3] >= 0)
	{
		t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
		t[0] = (t[0] < t[1] ? t[0] : t[1]);

		if (t[0] < *t0 && t[0] > 0)
		{
			*t0 = t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}

int		iraycone(t_ray *r, t_obj *obj, double *t0, t_objcomplement *comp)
{
	t_vec	delt_p;
	t_vec	tmp[3];
	double	abcd[4];
	bool	ret;

	ret = FALSE;
	delt_p = vectorinit(r->start.x - obj->pos.x, r->start.y - obj->pos.y,
	r->start.z - obj->pos.z);
	tmp[0] = vectorscale(vectordot(r->dir, obj->dir), obj->dir);
	tmp[1] = vectorsub(r->dir, tmp[0]);
	tmp[0] = vectorscale(vectordot(delt_p, obj->dir), obj->dir);
	tmp[2] = vectorsub(delt_p, tmp[0]);
	abcd[0] = (pow(cos(obj->alpha), 2)
	* vectordot(tmp[1], tmp[1])) - (pow(sin(obj->alpha), 2)
	* pow(vectordot(r->dir, obj->dir), 2));
	abcd[1] = 2 * ((pow(cos(obj->alpha), 2)
	* vectordot(tmp[1], tmp[2])) - (pow(sin(obj->alpha), 2)
	* vectordot(r->dir, obj->dir) * vectordot(delt_p, obj->dir)));
	abcd[2] = (pow(cos(obj->alpha), 2)
	* vectordot(tmp[2], tmp[2])) - (pow(sin(obj->alpha), 2)
	* pow(vectordot(delt_p, obj->dir), 2));
	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
	if (abcd[3] >= 0)
	{
		comp->t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		comp->t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);

		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0)
		{
			return(FALSE);
		}
		comp->normal = normalcone;

		//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
		if (obj->nextslice)
		{
			ret = irayslice(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (/*(t[0] > 0.001f) && */comp->t[0] < *t0 || *t0 == -1)
		{
			*t0 = comp->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}

int		irayplane(t_ray *r, t_obj *obj, double *t0, t_objcomplement *comp)
{
	t_vec	tmp;
	double	t[2];
	bool	ret;

	ret = FALSE;
	tmp = vectorsub(obj->pos, r->start);
	if (vectordot(obj->dir, r->dir) != 0)
	{
		t[0] = vectordot(obj->dir, tmp) / vectordot(obj->dir, r->dir);
	}
	else
	{
		return (FALSE);
	}

	comp->t[0] = t[0];
	comp->t[1] = DOESNOTEXIST;

	if (t[0] < 0)
	{
		return (FALSE);
	}
	comp->normal = normalplane;

	//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
	if (obj->nextslice)
	{
		ret = irayslice(r, obj, t0, comp);
		if (!ret)
		{
			return (FALSE);
		}
	}
	if (obj->nextneg)
	{
		ret = irayneg(r, obj, t0, comp);
		if (!ret)
		{
			return (FALSE);
		}
	}
	if (ret)
	{
		return (TRUE);
	}
	if (/*(t[0] > 0.001f) && */t[0] < *t0 || *t0 == -1)
	{
		*t0 = t[0];
		return (TRUE);
	}
	return (FALSE);
}

//Aq = Axd2 + Byd2 + Czd2 + Dxdyd + Exdzd + Fydzd
//Bq = 2*Axoxd + 2*Byoyd + 2*Czozd + D(xoyd + yoxd) + E(xozd + zoxd) + F(yozd + ydzo) + Gxd + Hyd + Izd
//Cq = Axo2 + Byo2 + Czo2 + Dxoyo + Exozo + Fyozo + Gxo + Hyo + Izo + J
int		irayquadric(t_ray *r, t_obj *obj, double *t0, t_objcomplement *comp)
{
	double	abcd[4];
	bool	ret;
	t_vec 	cam;

	//r->dir is wrong if there are reflections


	ret = FALSE;
	t_vec camdir = vectorsub(obj->pos, r->initialstart);
	double tmpdist = vectormagnitude(camdir);
	vectornormalize(&camdir);
	t_vec tmp_start = vectoradd(r->start, vectorscale(-1, vectorscale(tmpdist + SCREEN_EYE_DIST, camdir)));
	// the object is always at the origin, we simulate a distance between the eye and the object
	//to change it's position
	//	t_vec tmp_start = vectorinit(0, 0, 0);
	//	printf("tmp_start.x = %g\n"
	//		"tmp_start.y = %g\n"
	//		"tmp_start.z = %g\n", tmp_start.x, tmp_start.y, tmp_start.z);

	cam = r->dir;
	abcd[0] = obj->quad.a * pow(r->dir.x, 2);
	abcd[0] += obj->quad.b * pow(r->dir.y, 2);
	abcd[0] += obj->quad.c * pow(r->dir.z, 2);
	abcd[0] += obj->quad.f * r->dir.x * r->dir.y;
	abcd[0] += obj->quad.g * r->dir.x * r->dir.z;
	abcd[0] += obj->quad.h * r->dir.y * r->dir.z;

	abcd[1] = 2 * obj->quad.a * tmp_start.x * r->dir.x;
	abcd[1] += 2 * obj->quad.b * tmp_start.y * r->dir.y;
	abcd[1] += 2 * obj->quad.c * tmp_start.z * r->dir.z;
	abcd[1] += obj->quad.f * (tmp_start.x * r->dir.y + tmp_start.y * r->dir.x);
	abcd[1] += obj->quad.g * (tmp_start.x * r->dir.z + tmp_start.z * r->dir.x);
	abcd[1] += obj->quad.h * (tmp_start.y * r->dir.z + tmp_start.z * r->dir.y);
	abcd[1] += obj->quad.p * r->dir.x;
	abcd[1] += obj->quad.q * r->dir.y;
	abcd[1] += obj->quad.r * r->dir.z;

	abcd[2] = obj->quad.a * pow(tmp_start.x, 2);
	abcd[2] += obj->quad.b * pow(tmp_start.y, 2);
	abcd[2] += obj->quad.c * pow(tmp_start.z, 2);
	abcd[2] += obj->quad.f * tmp_start.x * tmp_start.y;
	abcd[2] += obj->quad.g * tmp_start.x * tmp_start.z;
	abcd[2] += obj->quad.h * tmp_start.y * tmp_start.z;
	abcd[2] += obj->quad.p * tmp_start.x;
	abcd[2] += obj->quad.q * tmp_start.y;
	abcd[2] += obj->quad.r * tmp_start.z;
	abcd[2] += -obj->quad.d;

	if (abcd[0] == 0) // solving first degree equation
	{
		comp->t[0] = -abcd[2] / abcd[1];
		if (comp->t[0] > 0 && comp->t[0] < *t0)
		{
			*t0 = comp->t[0];
			return (TRUE);
		}
		return (FALSE);
	}
	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
	if (abcd[3] >= 0)
	{
		comp->t[0] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
	//	if (comp->t[0] <= 0)
			comp->t[1] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
	//	else
	//		comp->t[1] = MAX_RANGE + 1;
		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0)
		{
			return(FALSE);
		}
		comp->normal = normalquadric;
		if (obj->nextslice)
		{
			ret = irayslice(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (/*(t[0] > 0.001f) && */comp->t[0] < *t0 || *t0 == -1)
		{
			*t0 = comp->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}

// (eyex + rdirx)^2 / dirx^2 + (eyey + rdiry)^2 / diry^2  + (eyey + rdiry)^2 / diry^2 - 1 = 0
//a = ray->start.x
//b = ray->start.y
//c = ray->start.z
//d = ray->dir.x
//e = ray->dir.y
//f = ray->dir.z
//g = cyl->dir.x
//h = cyl->dir.y
//i = cyl->dir.z

//Aqt2 + Bqt + Cq = 0 with

//Aq = Axd2 + Byd2 + Czd2 + Dxdyd + Exdzd + Fydzd

//Bq = 2*Axoxd + 2*Byoyd + 2*Czozd + D(xoyd + yoxd) + E(xozd + zoxd) + F(yozd + ydzo) + Gxd + Hyd + Izd

//Cq = Axo2 + Byo2 + Czo2 + Dxoyo + Exozo + Fyozo + Gxo + Hyo + Izo + J

int		iraycylinder(t_ray *r, t_obj *obj, double *t0, t_objcomplement *comp)
{
	double	abcd[4];
	t_vec	cam;
	bool	ret;

	ret = FALSE;
	cam = r->dir;
	t_vec origin = vectorsub(r->start, obj->pos);
	t_vec tmp1 = vectorsub(r->dir, vectorscale(vectordot(r->dir, obj->dir), obj->dir));
	t_vec tmp2 = vectorsub(origin, vectorscale(vectordot(origin, obj->dir), obj->dir));

	abcd[0] = vectordot(tmp1, tmp1);
	abcd[1] = 2 * vectordot(tmp1, tmp2);
	abcd[2] = vectordot(tmp2, tmp2) - pow(obj->rad, 2);
	abcd[3] = ft_pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);

	if (abcd[3] < 0)
		return (FALSE);
	else
	{
		comp->t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		comp->t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0)
		{
			return(FALSE);
		}
		comp->normal = normalcylinder;
		if (obj->nextslice)
		{
			ret = irayslice(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (/*(comp->t[0] > 0.001f) && */comp->t[0] < *t0 || *t0 == -1) // -1 only useful for neg
		{
			*t0 = comp->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}


//rotate slice

//returns a distance
int		irayslice(t_ray *r, t_obj *obj, double *dist, t_objcomplement *comp)
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
	if (comp->t[0] < 0 && comp->t[1] < 0)
	{
		return (FALSE);
	}
/*	if (comp->t[0] > comp->t[1])
	{
		tmptswitch = comp->t[0];
		comp->t[0] = comp->t[1];
		comp->t[1] = tmptswitch;
	}*/

	if (comp->t[0] > 0)
	{
		relativepos = vectoradd(r->start, vectorscale(comp->t[0], r->dir)); //tmpstart instead?
	}
	else
	{
		relativepos = vectoradd(r->start, vectorscale(comp->t[1], r->dir)); //tmpstart instead?
	}
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
			tmpt = vectordot(cursor->dir, tmp) / dot;

//			sliceinter = vectoradd(r->start, vectorscale(tmpt, r->dir));
//			relativedir = vectorsub(relativepos, sliceinter); //from slice intersection toward sphere intersection
//	printf("relativedir.x = %g relativedir.y = %g relativedir.z = %g\n", relativedir.x, relativedir.y, relativedir.z);
//			vectornormalize(&relativedir);
//	printf("relativepos.x = %g relativepos.y = %g relativepos.z = %g\n", relativepos.x, relativepos.y, relativepos.z);
//	printf("relativedir.x = %g relativedir.y = %g relativedir.z = %g\n", relativedir.x, relativedir.y, relativedir.z);
			//printf("tmpt = %g\nt0 = %g\n comp->t[1] = %g, dot = %g\n", tmpt, comp->t[0], comp->t[1], dot);
			//check if slice is inside the object
			//if not, maybe we're hitting nothing, or we're hitting the object
			if ((comp->t[0] > tmpt && comp->t[1] > tmpt) || (comp->t[0] < tmpt && comp->t[1] < tmpt))
			{
				//comp->t[0] will be the closest point
				/*if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || (comp->t[0] < 0 && comp->t[1] > 0))
				{
					tmptswitch = comp->t[0];
					comp->t[0] = comp->t[1];
				}*/
			//	else
			//	{
				//	return (FALSE); //no conceivable distance, i think
			//	}


				//object is fully in front of slice and slice is aligned on r->dir
				if (comp->t[1] < tmpt && dot > 0)
				{
					//return(FALSE);
				//	printf("comp->t[0] = %g, comp->t[1] = %g, tmpt = %g\n", comp->t[0], comp->t[1], tmpt);
					if (comp->t[0] > 0 && comp->t[0] <= *dist)
					{
				//		printf("Outcome1 reached\n");
						*dist = comp->t[0];
					}
					else if (comp->t[1] > 0 && comp->t[1] <= *dist)
					{
				//		printf("Outcome2 reached\n");
						*dist = comp->t[1];
					}
				//	return (TRUE);
				}//object is fully behind slice and slice is directed toward r->start
				else if (comp->t[0] > tmpt && dot < 0)
				{

					if (comp->t[0] > 0 && comp->t[0] <= *dist)
					{
						*dist = comp->t[0];
					}
					else if (comp->t[1] > 0 && comp->t[1] <= *dist)
					{
						*dist = comp->t[1];
					}
				}//object is sliced out, resulting in an absence of collision, I guess
				else// if (comp->t[0] != *dist && comp->t[1] != *dist)
				{
				//	*dist = tmptswitch;
				//	return (TRUE);
					return (FALSE);
				}
				//	return (TRUE); //slice is out of object
			}
			else if (dot > 0 /*vectordot(relativedir, cursor->dir) > 0*/) // ray coming toward sliced surface
			{
				//comp->t[0] will be the farthest point

				//	return(FALSE);
				if (comp->t[1] > 0)
				{
					*dist = comp->t[1];
				}
				else if (comp->t[0] > 0)
				{
					*dist = comp->t[0];
				}
				//return (TRUE);
				//				*dist = tmpt;
				//comp->type = TYPE_PLANE;
			}
			else if (dot < 0 /*vectordot(relativedir, cursor->dir) < 0*/) // ray coming toward object untouched surface
			{
				//comp->t[0] will be the closest point
				comp->reversen = TRUE; // not logic, but it works, need to check if it works everywhere..
				if (comp->t[0] > 0)
				{
					*dist = comp->t[0];
				}
				else if (comp->t[1] > 0)
				{
					*dist = comp->t[1];
				}
				//return (TRUE);
				//comp->type = TYPE_PLANE;
			}
		}
		else
		{
//			return (FALSE);
		}
		cursor = cursor->nextslice;
	}
				//	*dist = t0;
	return (TRUE);
}

//The problem lies in computeshadow, do my intersection works well when i go from the hole toward light?
int		irayneg(t_ray *r, t_obj *obj, double *dist, t_objcomplement *comp)
{
	t_obj	*cursor;
	t_obj	*deepestobj;
	double 	tmax;
	double	current;
	double  holet[2];
	t_objcomplement compbis;
//	t_vec sliceinter;

	holet[0] = 0;
	holet[1] = 0;
	cursor = obj->nextneg;
	//comp->normobj = NULL;
	compbis.reversen = FALSE;
	compbis.normal = NULL;
	compbis.normobj = NULL;
//	printf("slice->pos.x = %g, slice->pos.y = %g, slice->pos.z = %g\n", slice->pos.x, slice->pos.y, slice->pos.z);
	if (comp->t[0] < 0 && comp->t[1] < 0)
	{
		return (FALSE);
	}

	current = comp->t[0];
/*		printf("\n");

	deepestobj = cursor;
	while (deepestobj)
	{
		printf("type = %d, compbis.t[0] = %g, compbis.t[1] = %g, deepestobj->rad = %g, deepestobj->pos.x = %g, deepestobj->pos.y = %g, deepestobj->pos.z = %g, deepestobj->dir.x = %g, deepestobj->dir.y = %g, deepestobj->dir.z = %g\n", compbis.type, compbis.t[0], compbis.t[1], deepestobj->rad, deepestobj->pos.x, deepestobj->pos.y, deepestobj->pos.z, deepestobj->dir.x, deepestobj->dir.y, deepestobj->dir.z);
		deepestobj = deepestobj->nextitem;
	}*/

//printf("\n");
	while (cursor)
	{
		//printf("type = %d, compbis.t[0] = %g, compbis.t[1] = %g, holet[0] = %g, holet[1] = %g\n", cursor->type, compbis.t[0], compbis.t[1], holet[0], holet[1]);
		tmax = -1;
		if ((cursor->type == TYPE_SPHERE && iraysphere(r, cursor, &tmax, &compbis)) ||
			(cursor->type == TYPE_PLANE && irayplane(r, cursor, &tmax, &compbis)) ||
			(cursor->type == TYPE_CYLINDER && iraycylinder(r, cursor, &tmax, &compbis)) ||
			(cursor->type == TYPE_CONE && iraycone(r, cursor, &tmax, &compbis)) ||
			(cursor->type == TYPE_QUADRIC && irayquadric(r, cursor, &tmax, &compbis)) ||
			 (cursor->type == TYPE_TORUS && iraytorus(r, cursor, &tmax, &compbis)))
		{
			if (compbis.t[1] == DOESNOTEXIST)
			{
				continue;
			}
			if (compbis.t[0] > compbis.t[1]) //doesnt matter if holet[0] is inferior to 0 here
			{
				swapdouble(&compbis.t[0], &compbis.t[1]);
			}
			//first negative object
			if (holet[0] == 0 && holet[1] == 0)
			{
				deepestobj = cursor;
			//	comp->normobj = cursor;
			//	comp->normal = cursor->normal;
				holet[0] = compbis.t[0];
				holet[1] = compbis.t[1];
			//	if (holet[0] > holet[1]) //doesnt matter if holet[0] is inferior to 0 here
			//	{
			//		swapdouble(&holet[0], &holet[1]);
			//	}
			}
			else if (holet[0] <= compbis.t[1] && compbis.t[0] <= holet[1]) // check for overlap
			{
				if (compbis.t[0] < holet[0])
				{
					holet[0] = compbis.t[0];
				}
				if (compbis.t[1] > holet[1])
				{
					deepestobj = cursor;
				//	comp->normobj = cursor;
				//	comp->normal = cursor->normal;
					holet[1] = compbis.t[1];
				}
			}
		}
		//printf("type = %d, compbis.t[0] = %g, compbis.t[1] = %g, holet[0] = %g, holet[1] = %g\n", cursor->type, compbis.t[0], compbis.t[1], holet[0], holet[1]);
		//it should be nextitem and not nextneg
		cursor = cursor->nextitem;
	}
		//case 1 hole going through object
		//case 2 hole in front object
		//case 3 hole behind object
//	if (e->x == 355 && e->y == 263)
//	{
//		printf("case 0 = holet[0] = %g, holet[1] = %g, comp->t[0] = %g, comp->t[1] = %g\n", holet[0], holet[1], comp->t[0], comp->t[1]);
//	}
//if we are inside the object how do we determine how to
	if (holet[0] < comp->t[0] && holet[1] > comp->t[1])
	{
//	if (e->x == 355 && e->y == 263)
//		printf("case 1 = holet[0] = %g, holet[1] = %g, comp->t[0] = %g, comp->t[1] = %g\n", holet[0], holet[1], comp->t[0], comp->t[1]);
		return (FALSE);
	}
	else if (holet[0] < comp->t[0]/* && holet[1] < comp->t[1]*/)
	{
//	if (e->x == 355 && e->y == 263)
//		printf("case 2 = holet[0] = %g, holet[1] = %g, comp->t[0] = %g, comp->t[1] = %g\n", holet[0], holet[1], comp->t[0], comp->t[1]);
		if (holet[1] > current)
		{
			comp->normobj = deepestobj;
			comp->normal = compbis.normal;
			if (comp->normobj)
			{
			//	comp->normobj->reversen = TRUE;
				comp->reversen = TRUE;
			}
			current = holet[1]; //we need the deepest hole
		}
	}
				//	*dist = t0;
	*dist = current;
	return (TRUE);
}


//negative object model working with shadow
//a negative object going through an object (t0 and t1 of negative larger than both t0 and t1 of native object)
int		iraysphere(t_ray *r, t_obj *obj, double *t0, t_objcomplement *comp)
{
	double	abcdiscr[5];
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
		comp->t[0] = (-(abcdiscr[1]) + abcdiscr[4]) / (2);
		comp->t[1] = (-(abcdiscr[1]) - abcdiscr[4]) / (2);

		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0) // this is not necesarily true, if i have a negative object, it can extend T and bring it into view
		{
			return(FALSE);
		}
/*		else if (t[0] < 0)
		{
			t[0] = t[1];
		}*/

		comp->normal = normalsphere;

		//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
		if (obj->nextslice)
		{
			ret = irayslice(r, obj, t0, comp);

			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}

		if (/*(t[0] > 0.001f) && */(comp->t[0] < *t0) || *t0 == -1)
		{
		//printf("tfinal = %g\n", t[0]);

			*t0 = comp->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}

/*double	quarticsolver(double p[5])
{
	double discr;

	discr = 256 * a^3 * e^3 - 192 * a^2 * b * d * e^2 - 128 * a^2 * c^2 * e^2 + 144 * a^2 * c * d^2 * e - 27 * a ^2 * d^4
			+ 144 * a * b^2 * c * e^2 - - 6 * a * b^2 * d^2 * e - 80



	double Q;
	double S;
	double theta0;
	double theta1;

	theta0 = c^2 - 3 * b * d + 12 * a * e;
	theta1 = 2 * c^3 - 9 * b *c * d + 27 * b^2 * e + 27 * a * d^2 - 72 * a * c * e;


	return(0);
}*/

static double _root3 ( double x )
{
    double s = 1.;
    while ( x < 1. )
    {
        x *= 8.;
        s *= 0.5;
    }
    while ( x > 8. )
    {
        x *= 0.125;
        s *= 2.;
    }
    double r = 1.5;
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    return r * s;
}

double root3 ( double x )
{
    if ( x > 0 ) return _root3 ( x ); else
    if ( x < 0 ) return-_root3 (-x ); else
    return 0.;
}


// x - array of size 2
// return 2: 2 real roots x[0], x[1]
// return 0: pair of complex roots: x[0]±i*x[1]
int   SolveP2(double *x, double a, double b) {			// solve equation x^2 + a*x + b = 0
	double D = 0.25*a*a - b;
	if (D >= 0) {
		D = sqrt(D);
		x[0] = 0.5*a + D;
		x[1] = 0.5*a - D;
		return 2;
	}
	x[0] = 0.5*a;
	x[1] = sqrt(-D);
	return 0;
}
//---------------------------------------------------------------------------
// x - array of size 3
// In case 3 real roots: => x[0], x[1], x[2], return 3
//         2 real roots: x[0], x[1],          return 2
//         1 real root : x[0], x[1] ± i*x[2], return 1
int SolveP3(double *x,double a,double b,double c) {	// solve cubic equation x^3 + a*x^2 + b*x + c = 0
	double a2 = a*a;
    double q  = (a2 - 3*b)/9; 
	double r  = (a*(2*a2-9*b) + 27*c)/54;
	// equation x^3 + q*x + r = 0
    double r2 = r*r;
	double q3 = q*q*q;
	double A,B;
    if(r2<q3) {
        double t=r/sqrt(q3);
		if( t<-1) t=-1;
		if( t> 1) t= 1;
        t=acos(t);
        a/=3; q=-2*sqrt(q);
        x[0]=q*cos(t/3)-a;
        x[1]=q*cos((t+TwoPi)/3)-a;
        x[2]=q*cos((t-TwoPi)/3)-a;
        return(3);
    } else {
        //A =-pow(fabs(r)+sqrt(r2-q3),1./3); 
        A =-root3(fabs(r)+sqrt(r2-q3)); 
		if( r<0 ) A=-A;
		B = A==0? 0 : q/A;

		a/=3;
		x[0] =(A+B)-a;
        x[1] =-0.5*(A+B)-a;
        x[2] = 0.5*sqrt(3.)*(A-B);
		if(fabs(x[2])<KEPSILON) { x[2]=x[1]; return(2); }
        return(1);
    }
}// SolveP3(double *x,double a,double b,double c) {	
//---------------------------------------------------------------------------
// a>=0!
void  CSqrt( double x, double y, double *a, double *b) // returns:  (*a)+i*s = sqrt(x+i*y)
{
	double r  = sqrt(x*x+y*y);
	if( y==0 ) { 
		r = sqrt(r);
		if(x>=0) { (*a)=r; (*b)=0; } else { (*a)=0; (*b)=r; }
	} else {		// y != 0
		(*a) = sqrt(0.5*(x+r));
		(*b) = 0.5*y/(*a);
	}
}
//---------------------------------------------------------------------------
int   SolveP4Bi(double *x, double b, double d)	// solve equation x^4 + b*x^2 + d = 0
{
	double D = b*b-4*d;
	if( D>=0 ) 
	{
		double sD = sqrt(D);
		double x1 = (-b+sD)/2;
		double x2 = (-b-sD)/2;	// x2 <= x1
		if( x2>=0 )				// 0 <= x2 <= x1, 4 real roots
		{
			double sx1 = sqrt(x1);
			double sx2 = sqrt(x2);
			x[0] = -sx1;
			x[1] =  sx1;
			x[2] = -sx2;
			x[3] =  sx2;
			return 4;
		}
		if( x1 < 0 )				// x2 <= x1 < 0, two pair of imaginary roots
		{
			double sx1 = sqrt(-x1);
			double sx2 = sqrt(-x2);
			x[0] =    0;
			x[1] =  sx1;
			x[2] =    0;
			x[3] =  sx2;
			return 0;
		}
		// now x2 < 0 <= x1 , two real roots and one pair of imginary root
			double sx1 = sqrt( x1);
			double sx2 = sqrt(-x2);
			x[0] = -sx1;
			x[1] =  sx1;
			x[2] =    0;
			x[3] =  sx2;
			return 2;
	} else { // if( D < 0 ), two pair of compex roots
		double sD2 = 0.5*sqrt(-D);
		CSqrt(-0.5*b, sD2, &x[0],&x[1]);
		CSqrt(-0.5*b,-sD2, &x[2],&x[3]);
		return 0;
	} // if( D>=0 ) 
} // SolveP4Bi(double *x, double b, double d)	// solve equation x^4 + b*x^2 d
//---------------------------------------------------------------------------
static void  dblSort3( double *a, double *b, double *c) // make: a <= b <= c
{
	if( (*a)>(*b) ) swapdouble(a, b);	// now a<=(*b)
	if( (*c)<(*b) ) {
		swapdouble(b,c);			// now a<=(*b), (*b)<=c
		if( (*a)>(*b) ) swapdouble(a, b);// now a<=b
	}
}
//---------------------------------------------------------------------------
int   SolveP4De(double *x, double b, double c, double d)	// solve equation x^4 + b*x^2 + c*x + d
{
	//if( c==0 ) return SolveP4Bi(x,b,d); // After that, c!=0
	if( fabs(c)<1e-14*(fabs(b)+fabs(d)) ) return SolveP4Bi(x,b,d); // After that, c!=0

	int res3 = SolveP3( x, 2*b, b*b-4*d, -c*c);	// solve resolvent
	// by Viet theorem:  x1*x2*x3=-c*c not equals to 0, so x1!=0, x2!=0, x3!=0
	if( res3>1 )	// 3 real roots, 
	{				
		dblSort3(&x[0], &x[1], &x[2]);	// sort roots to x[0] <= x[1] <= x[2]
		// Note: x[0]*x[1]*x[2]= c*c > 0
		if( x[0] > 0) // all roots are positive
		{
			double sz1 = sqrt(x[0]);
			double sz2 = sqrt(x[1]);
			double sz3 = sqrt(x[2]);
			// Note: sz1*sz2*sz3= -c (and not equal to 0)
			if( c>0 )
			{
				x[0] = (-sz1 -sz2 -sz3)/2;
				x[1] = (-sz1 +sz2 +sz3)/2;
				x[2] = (+sz1 -sz2 +sz3)/2;
				x[3] = (+sz1 +sz2 -sz3)/2;
				return 4;
			}
			// now: c<0
			x[0] = (-sz1 -sz2 +sz3)/2;
			x[1] = (-sz1 +sz2 -sz3)/2;
			x[2] = (+sz1 -sz2 -sz3)/2;
			x[3] = (+sz1 +sz2 +sz3)/2;
			return 4;
		} // if( x[0] > 0) // all roots are positive
		// now x[0] <= x[1] < 0, x[2] > 0
		// two pair of comlex roots
		double sz1 = sqrt(-x[0]);
		double sz2 = sqrt(-x[1]);
		double sz3 = sqrt( x[2]);

		if( c>0 )	// sign = -1
		{
			x[0] = -sz3/2;					
			x[1] = ( sz1 -sz2)/2;		// x[0]±i*x[1]
			x[2] =  sz3/2;
			x[3] = (-sz1 -sz2)/2;		// x[2]±i*x[3]
			return 0;
		}
		// now: c<0 , sign = +1
		x[0] =   sz3/2;
		x[1] = (-sz1 +sz2)/2;
		x[2] =  -sz3/2;
		x[3] = ( sz1 +sz2)/2;
		return 0;
	} // if( res3>1 )	// 3 real roots, 
	// now resoventa have 1 real and pair of compex roots
	// x[0] - real root, and x[0]>0, 
	// x[1]±i*x[2] - complex roots, 
	// x[0] must be >=0. But one times x[0]=~ 1e-17, so:
	if (x[0] < 0) x[0] = 0;
	double sz1 = sqrt(x[0]);
	double szr, szi;
	CSqrt(x[1], x[2], &szr, &szi);  // (szr+i*szi)^2 = x[1]+i*x[2]
	if( c>0 )	// sign = -1
	{
		x[0] = -sz1/2-szr;			// 1st real root
		x[1] = -sz1/2+szr;			// 2nd real root
		x[2] = sz1/2; 
		x[3] = szi;
		return 2;
	}
	// now: c<0 , sign = +1
	x[0] = sz1/2-szr;			// 1st real root
	x[1] = sz1/2+szr;			// 2nd real root
	x[2] = -sz1/2;
	x[3] = szi;
	return 2;
} // SolveP4De(double *x, double b, double c, double d)	// solve equation x^4 + b*x^2 + c*x + d
//-----------------------------------------------------------------------------
double N4Step(double x, double a,double b,double c,double d)	// one Newton step for x^4 + a*x^3 + b*x^2 + c*x + d
{
	double fxs= ((4*x+3*a)*x+2*b)*x+c;	// f'(x)
	if( fxs==0 ) return 1e99;
	double fx = (((x+a)*x+b)*x+c)*x+d;	// f(x)
	return x - fx/fxs;
} 
//-----------------------------------------------------------------------------
// x - array of size 4
// return 4: 4 real roots x[0], x[1], x[2], x[3], possible multiple roots
// return 2: 2 real roots x[0], x[1] and complex x[2]±i*x[3], 
// return 0: two pair of complex roots: x[0]±i*x[1],  x[2]±i*x[3], 
int   SolveP4(double x[4],double a,double b,double c,double d) {	// solve equation x^4 + a*x^3 + b*x^2 + c*x + d by Dekart-Euler method
	// move to a=0:
	double d1 = d + 0.25*a*( 0.25*b*a - 3./64*a*a*a - c);
	double c1 = c + 0.5*a*(0.25*a*a - b);
	double b1 = b - 0.375*a*a;
	int res = SolveP4De( x, b1, c1, d1);
	if( res==4) { x[0]-= a/4; x[1]-= a/4; x[2]-= a/4; x[3]-= a/4; }
	else if (res==2) { x[0]-= a/4; x[1]-= a/4; x[2]-= a/4; }
	else             { x[0]-= a/4; x[2]-= a/4; }
	// one Newton step for each real root:
	if( res>0 )
	{
		x[0] = N4Step(x[0], a,b,c,d);
		x[1] = N4Step(x[1], a,b,c,d);
	}
	if( res>2 )
	{
		x[2] = N4Step(x[2], a,b,c,d);
		x[3] = N4Step(x[3], a,b,c,d);
	}
	return res;
}
	// D = ray direction
	// V = obj->dir
	// X = ray origin - obj pos

	//m = D|D
	//n = D|X
	//o = X|X
	//p = D|V
	//q = X|V

	//a = m^2
	//b = 4*m*n
	//c = 4*m^2 + 2*m*o - 2*(R^2+r^2)*m + 4*R^2*p^2
	//d = 4*n*o - 4*(R^2+r^2)*n + 8*R^2*p*q
	//e = o^2 - 2*(R^2+r^2)*o + 4*R^2*q^2 + (R^2-r^2)^2


//negative object model working with shadow
//a negative object going through an object (t0 and t1 of negative larger than both t0 and t1 of native object)
int		iraytorus(t_ray *r, t_obj *obj, double *t0, t_objcomplement *comp)
{
	bool	ret;
	double	abcdiscr[5];
//	double	tmp[5];
//	t_vec	dist;
	double	t[4];
	int 	nb_root;

	t_vec rayOriginPosition = r->start;
	t_vec rayDirection = r->dir;

	t_vec centerToRayOrigin = vectorsub(rayOriginPosition, obj->pos);
	const double centerToRayOriginDotDirection = vectordot(rayDirection, centerToRayOrigin);
	double	centerToRayOriginDotDirectionSquared = vectordot(centerToRayOrigin, centerToRayOrigin);
	double innerRadiusSquared = obj->rad2 * obj->rad2;
	double outerRadiusSquared = obj->rad * obj->rad;

	double	axisDotCenterToRayOrigin	= vectordot(obj->dir, centerToRayOrigin);
	double	axisDotRayDirection = vectordot(obj->dir, rayDirection);
	double	a = 1 - axisDotRayDirection * axisDotRayDirection;
	double	b = 2 * (vectordot(centerToRayOrigin, rayDirection) - axisDotCenterToRayOrigin * axisDotRayDirection);
	double c = centerToRayOriginDotDirectionSquared - axisDotCenterToRayOrigin * axisDotCenterToRayOrigin;
	double	d = centerToRayOriginDotDirectionSquared + outerRadiusSquared - innerRadiusSquared;

	// Solve quartic equation with coefficients A, B, C, D and E
	abcdiscr[0] = 1; 
	abcdiscr[1] = 4 * centerToRayOriginDotDirection;
	abcdiscr[2] = 2 * d + abcdiscr[1] * abcdiscr[1] * 0.25f - 4 * outerRadiusSquared * a;
	abcdiscr[3] = abcdiscr[1] * d - 4 * outerRadiusSquared * b;
	abcdiscr[4] = d * d - 4 * outerRadiusSquared * c;
//	abcdiscr[1] /= abcdiscr[0];
//	abcdiscr[2] /= abcdiscr[0];
//	abcdiscr[3] /= abcdiscr[0];
//	abcdiscr[4] /= abcdiscr[0];

	nb_root = SolveP4(t, abcdiscr[1], abcdiscr[2], abcdiscr[3], abcdiscr[4]);

	if (nb_root)
	{
		comp->t[0] = t[0];
		comp->t[1] = t[1];

		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0) // this is not necesarily true, if i have a negative object, it can extend T and bring it into view
		{
			return(FALSE);
		}


		comp->normal = normaltorus;

		//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
		if (obj->nextslice)
		{
			ret = irayslice(r, obj, t0, comp);

			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}

		if ((comp->t[0] < *t0) || *t0 == -1)
		{
		//printf("tfinal = %g\n", t[0]);

			*t0 = comp->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}
