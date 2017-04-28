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

int		iraycone(t_ray *r, t_obj *obj, double *t0)
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
		obj->t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		obj->t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);

		if ((obj->t[0] > obj->t[1] && obj->t[1] > 0) || obj->t[0] < 0)
		{
			swapdouble(&obj->t[0], &obj->t[1]);
		}

		if (obj->t[0] < 0 && obj->t[1] < 0)
		{
			return(FALSE);
		}
		obj->specificnormal = TYPE_CONE;

		//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
		if (obj->nextslice[0].set == TRUE)
		{
			ret = irayslice(r, obj, t0);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg[0].set == TRUE)
		{
			ret = irayneg(r, obj, t0);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (/*(t[0] > 0.001f) && */obj->t[0] < *t0)
		{
			*t0 = obj->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}

int		irayplane(t_ray *r, t_obj *obj, double *t0)
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

	obj->t[0] = t[0];
	obj->t[1] = DOESNOTEXIST;

	if (t[0] < 0)
	{
		return (FALSE);
	}

	//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
	obj->specificnormal = TYPE_PLANE;
	if (obj->nextslice[0].set == TRUE)
	{
		ret = irayslice(r, obj, t0);
		if (!ret)
		{
			return (FALSE);
		}
	}
	if (obj->nextneg[0].set == TRUE)
	{
		ret = irayneg(r, obj, t0);
		if (!ret)
		{
			return (FALSE);
		}
	}
	if (ret)
	{
		return (TRUE);
	}
	if (/*(t[0] > 0.001f) && */t[0] < *t0)
	{
		*t0 = t[0];
		return (TRUE);
	}
	return (FALSE);
}

//Aq = Axd2 + Byd2 + Czd2 + Dxdyd + Exdzd + Fydzd
//Bq = 2*Axoxd + 2*Byoyd + 2*Czozd + D(xoyd + yoxd) + E(xozd + zoxd) + F(yozd + ydzo) + Gxd + Hyd + Izd
//Cq = Axo2 + Byo2 + Czo2 + Dxoyo + Exozo + Fyozo + Gxo + Hyo + Izo + J
int		irayquadric(t_ray *r, t_obj *obj, double *t0)
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
		obj->t[0] = -abcd[2] / abcd[1];
		if (obj->t[0] > 0 && obj->t[0] < *t0)
		{
			*t0 = obj->t[0];
			return (TRUE);
		}
		return (FALSE);
	}
	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
	if (abcd[3] >= 0)
	{
		obj->t[0] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
	//	if (obj->t[0] <= 0)
			obj->t[1] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
	//	else
	//		obj->t[1] = MAX_RANGE + 1;
		if ((obj->t[0] > obj->t[1] && obj->t[1] > 0) || obj->t[0] < 0)
		{
			swapdouble(&obj->t[0], &obj->t[1]);
		}

		if (obj->t[0] < 0 && obj->t[1] < 0)
		{
			return(FALSE);
		}
		obj->specificnormal = TYPE_QUADRIC;
		if (obj->nextslice[0].set == TRUE)
		{
			ret = irayslice(r, obj, t0);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg[0].set == TRUE)
		{
			ret = irayneg(r, obj, t0);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (/*(t[0] > 0.001f) && */obj->t[0] < *t0)
		{
			*t0 = obj->t[0];
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

int		iraycylinder(t_ray *r, t_obj *obj, double *t0)
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
	{
		return (FALSE);
	}
	else
	{
		obj->t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		obj->t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
		if ((obj->t[0] > obj->t[1] && obj->t[1] > 0) || obj->t[0] < 0)
		{
			swapdouble(&obj->t[0], &obj->t[1]);
		}

		if (obj->t[0] < 0 && obj->t[1] < 0)
		{
			return(FALSE);
		}
		obj->specificnormal = TYPE_CYLINDER;
		if (obj->nextslice[0].set == TRUE)
		{
			ret = irayslice(r, obj, t0);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (obj->nextneg[0].set == TRUE)
		{
			ret = irayneg(r, obj, t0);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (/*(obj->t[0] > 0.001f) && */obj->t[0] < *t0)
		{
			*t0 = obj->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}


//rotate slice

//returns a distance
int		irayslice(t_ray *r, t_obj *obj, double *dist)
{
	t_vec	tmp;
	double	tmpt;
	int 	i;
	double	dot;

	t_vec relativepos; //position of intersection
	t_vec relativedir;
//	t_vec sliceinter;

//	printf("slice->pos.x = %g, slice->pos.y = %g, slice->pos.z = %g\n", slice->pos.x, slice->pos.y, slice->pos.z);
	if (obj->t[0] < 0 && obj->t[1] < 0)
	{
		return (FALSE);
	}

	if (obj->t[0] > 0)
	{
		relativepos = vectoradd(r->start, vectorscale(obj->t[0], r->dir)); //tmpstart instead?
	}
	else
	{
		relativepos = vectoradd(r->start, vectorscale(obj->t[1], r->dir)); //tmpstart instead?
	}
	relativepos = vectorsub(obj->pos, relativepos); //now in local space?
	relativedir = /*vectorsub(cursor->pos, */relativepos/*)*/; //from local center position to intersection
	vectornormalize(&relativedir);
//relative dir should be a vector going from the slice toward emptiness/sliced part

	i = 0;
	while (obj->nextslice[i].set == TRUE)
	{
//case 0 =  unreachable slice, 2 cases, touching object or touching void
//case 1 =  intersection with plan, facing plan
//case 2 =  intersection with plan, not facing plan

//case 3 =  no intersection with plan and good dir
//case 3 =  no intersection with plan and wrong dir
//		printf("cursor->dir.x = %g cursor->dir.y = %g cursor->dir.z = %g\n", cursor->dir.x, cursor->dir.y, cursor->dir.z);
		tmp = vectorsub(vectoradd(obj->nextslice[i].pos, obj->pos), r->start);
		dot = vectordot(obj->nextslice[i].dir, r->dir);
		if (dot)
		{
			tmpt = vectordot(obj->nextslice[i].dir, tmp) / dot;
			//check if slice is inside the object
			//if not, maybe we're hitting nothing, or we're hitting the object
			if ((obj->t[0] > tmpt && obj->t[1] > tmpt) || (obj->t[0] < tmpt && obj->t[1] < tmpt))
			{
				//object is fully in front of slice and slice is aligned on r->dir
				if (obj->t[1] < tmpt && dot > 0)
				{
					//return(FALSE);
				//	printf("obj->t[0] = %g, obj->t[1] = %g, tmpt = %g\n", obj->t[0], obj->t[1], tmpt);
					if (obj->t[0] > 0 && obj->t[0] <= *dist)
					{
						*dist = obj->t[0];
					}
					else if (obj->t[1] > 0 && obj->t[1] <= *dist)
					{
						*dist = obj->t[1];
					}
				}//object is fully behind slice and slice is directed toward r->start
				else if (obj->t[0] > tmpt && dot < 0)
				{
					if (obj->t[0] > 0 && obj->t[0] <= *dist)
					{
						*dist = obj->t[0];
					}
					else if (obj->t[1] > 0 && obj->t[1] <= *dist)
					{
						*dist = obj->t[1];
					}
				}//object is sliced out, resulting in an absence of collision, I guess
				else// if (obj->t[0] != *dist && obj->t[1] != *dist)
				{
					return (FALSE);
				}
				//	return (TRUE); //slice is out of object
			}
			else if (dot > 0) // ray coming toward sliced surface
			{
				//obj->t[0] will be the farthest point

				//	return(FALSE);
				if (obj->t[1] > 0)
				{
					*dist = obj->t[1];
				}
				else if (obj->t[0] > 0)
				{
					*dist = obj->t[0];
				}
			}
			else if (dot < 0 /*vectordot(relativedir, cursor->dir) < 0*/) // ray coming toward object untouched surface
			{
				//obj->t[0] will be the closest point
				obj->reversen = TRUE; // not logic, but it works, need to check if it works everywhere..
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
		++i;
	}
	return (TRUE);
}

//The problem lies in computeshadow, do my intersection works well when i go from the hole toward light?
int		irayneg(t_ray *r, t_obj *obj, double *dist)
{
//	t_obj	*cursor;
	int 	deepestobj;
	double 	tmax;
	double	current;
	double  holet[2];
	int i;
//	t_vec sliceinter;

			//	ft_putendl("unhandled case irayneg");
	i = 0;
	holet[0] = 0;
	holet[1] = 0;
	deepestobj = -1;
//	cursor = obj->nextneg;
	obj->normobj.set = FALSE;
//	printf("slice->pos.x = %g, slice->pos.y = %g, slice->pos.z = %g\n", slice->pos.x, slice->pos.y, slice->pos.z);
	if (obj->t[0] < 0 && obj->t[1] < 0)
	{
		return (FALSE);
	}

	current = obj->t[0];
/*		printf("\n");

	deepestobj = cursor;
	while (deepestobj)
	{
		printf("type = %d, deepestobj->t[0] = %g, deepestobj->t[1] = %g, deepestobj->rad = %g, deepestobj->pos.x = %g, deepestobj->pos.y = %g, deepestobj->pos.z = %g, deepestobj->dir.x = %g, deepestobj->dir.y = %g, deepestobj->dir.z = %g\n", deepestobj->type, deepestobj->t[0], deepestobj->t[1], deepestobj->rad, deepestobj->pos.x, deepestobj->pos.y, deepestobj->pos.z, deepestobj->dir.x, deepestobj->dir.y, deepestobj->dir.z);
		deepestobj = deepestobj->nextitem;
	}*/

//printf("\n");
	while (obj->nextneg[i].set == TRUE)
	{
		//printf("type = %d, cursor->t[0] = %g, cursor->t[1] = %g, holet[0] = %g, holet[1] = %g\n", cursor->type, cursor->t[0], cursor->t[1], holet[0], holet[1]);
	//	printf("obj->nextneg[i].pos.x = %g, obj->nextneg[i].pos.y = %g, obj->nextneg[i].pos.z = %g\n", obj->nextneg[i].pos.x, obj->nextneg[i].pos.y, obj->nextneg[i].pos.z);
	//	printf("obj->nextneg[i].dir.x = %g, obj->nextneg[i].dir.y = %g, obj->nextneg[i].dir.z = %g\n", obj->nextneg[i].dir.x, obj->nextneg[i].dir.y, obj->nextneg[i].dir.z);
		tmax = MAX_RANGE;
		if ((obj->nextneg[i].type == TYPE_SPHERE && iraysphere(r, (t_obj*)(&obj->nextneg[i]), &tmax)) ||
			(obj->nextneg[i].type == TYPE_PLANE && irayplane(r, (t_obj*)(&obj->nextneg[i]), &tmax)) ||
			(obj->nextneg[i].type == TYPE_CYLINDER && iraycylinder(r, (t_obj*)(&obj->nextneg[i]), &tmax)) ||
			(obj->nextneg[i].type == TYPE_CONE && iraycone(r, (t_obj*)(&obj->nextneg[i]), &tmax)) ||
			(obj->nextneg[i].type == TYPE_QUADRIC && irayquadric(r, (t_obj*)(&obj->nextneg[i]), &tmax)))
		{
			if (obj->nextneg[i].t[1] == DOESNOTEXIST)
			{
				continue;
			}
			if (obj->nextneg[i].t[0] > obj->nextneg[i].t[1]) //doesnt matter if holet[0] is inferior to 0 here
			{
				swapdouble(&obj->nextneg[i].t[0], &obj->nextneg[i].t[1]);
			}
			//first negative object
			if (holet[0] == 0 && holet[1] == 0)
			{
				deepestobj = i;
			//	obj->normobj = obj->nextneg[i];
			//	obj->normal = obj->nextneg[i]->normal;
				holet[0] = obj->nextneg[i].t[0];
				holet[1] = obj->nextneg[i].t[1];
			//	if (holet[0] > holet[1]) //doesnt matter if holet[0] is inferior to 0 here
			//	{
			//		swapdouble(&holet[0], &holet[1]);
			//	}
			}
			else if (holet[0] <= obj->nextneg[i].t[1] && obj->nextneg[i].t[0] <= holet[1]) // check for overlap
			{
				if (obj->nextneg[i].t[0] < holet[0])
				{
					holet[0] = obj->nextneg[i].t[0];
				}
				if (obj->nextneg[i].t[1] > holet[1])
				{
					deepestobj = i;
				//	obj->normobj = obj->nextneg[i];
				//	obj->normal = obj->nextneg[i]->normal;
					holet[1] = obj->nextneg[i].t[1];
				}
			}
		}
		//printf("type = %d, obj->nextneg[i]->t[0] = %g, obj->nextneg[i]->t[1] = %g, holet[0] = %g, holet[1] = %g\n", obj->nextneg[i]->type, obj->nextneg[i]->t[0], obj->nextneg[i]->t[1], holet[0], holet[1]);
		//it should be nextitem and not nextneg
		++i;
	}
		//case 1 hole going through object
		//case 2 hole in front object
		//case 3 hole behind object
//	if (e->x == 355 && e->y == 263)
//	{
//		printf("case 0 = holet[0] = %g, holet[1] = %g, obj->t[0] = %g, obj->t[1] = %g\n", holet[0], holet[1], obj->t[0], obj->t[1]);
//	}
//if we are inside the object how do we determine how to
	if (holet[0] < obj->t[0] && holet[1] > obj->t[1])
	{
//	if (e->x == 355 && e->y == 263)
//		printf("case 1 = holet[0] = %g, holet[1] = %g, obj->t[0] = %g, obj->t[1] = %g\n", holet[0], holet[1], obj->t[0], obj->t[1]);
		return (FALSE);
	}
	else if (holet[0] < obj->t[0]/* && holet[1] < obj->t[1]*/)
	{
	//	ft_putendl("case 2");
//	if (e->x == 355 && e->y == 263)
//		printf("case 2 = holet[0] = %g, holet[1] = %g, obj->t[0] = %g, obj->t[1] = %g\n", holet[0], holet[1], obj->t[0], obj->t[1]);
		if (holet[1] > current && deepestobj != -1)
		{
			ft_memcpy(&obj->normobj, &obj->nextneg[deepestobj], sizeof(t_neg));
		//	obj->normal = deepestobj->normal;
			obj->normobj.set = TRUE;
			obj->normobj.reversen = TRUE;
			current = holet[1]; //we need the deepest hole
		}
	}

	//	ft_putendl("ok6");
				//	*dist = t0;
	*dist = current;
	return (TRUE);
}


//negative object model working with shadow
//a negative object going through an object (t0 and t1 of negative larger than both t0 and t1 of native object)
int		iraysphere(t_ray *r, t_obj *obj, double *t0)
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
		obj->t[0] = (-(abcdiscr[1]) + abcdiscr[4]) / (2);
		obj->t[1] = (-(abcdiscr[1]) - abcdiscr[4]) / (2);

		if ((obj->t[0] > obj->t[1] && obj->t[1] > 0) || obj->t[0] < 0)
		{
			swapdouble(&obj->t[0], &obj->t[1]);
		}

		if (obj->t[0] < 0 && obj->t[1] < 0) // this is not necesarily true, if i have a negative object, it can extend T and bring it into view
		{
			return(FALSE);
		}
		obj->specificnormal = TYPE_SPHERE;
/*		else if (t[0] < 0)
		{
			t[0] = t[1];
		}*/

		//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
		if (obj->nextslice[0].set == TRUE)
		{
			ret = irayslice(r, obj, t0);

			if (!ret)
			{
				return (FALSE);
			}
		}

		if (obj->nextneg[0].set == TRUE)
		{
	//		printf("%d\n", obj->nextneg[0].set);
			ret = irayneg(r, obj, t0);
			if (!ret)
			{
				return (FALSE);
			}
	//		printf("%d\n", obj->nextneg[0].set);
		}
		if (ret)
		{
			return (TRUE);
		}

		if (/*(t[0] > 0.001f) && */(obj->t[0] < *t0))
		{
		//printf("tfinal = %g\n", t[0]);

			*t0 = obj->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}
