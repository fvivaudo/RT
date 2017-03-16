/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:58:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:38:40 by dmoureu-         ###   ########.fr       */
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
		if ((cursor->type == TYPE_SPHERE && iraysphere(r, cursor, &t, e)) ||
			(cursor->type == TYPE_PLANE && irayplane(r, cursor, &t, e)) ||
			(cursor->type == TYPE_CYLINDER && iraycylinder(r, cursor, &t, e)) ||
			(cursor->type == TYPE_CONE && iraycone(r, cursor, &t, e)) ||
			(cursor->type == TYPE_QUADRIC && irayquadric(r, cursor, &t, e)))
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
		if (res->normobj)
			res->normal(e, res->normobj);
		else
			res->normal(e, res);
	}
	return (res);
}

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

int		iraycone(t_ray *r, t_obj *obj, double *t0, t_env *e)
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

		if (obj->t[0] > obj->t[1])
		{
			swapdouble(&obj->t[0], &obj->t[1]);
		}

		if (obj->t[0] < 0 && obj->t[1] < 0)
		{
			return(FALSE);
		}
		obj->normal = normalcone;

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
		if (/*(t[0] > 0.001f) && */obj->t[0] < *t0)
		{
			*t0 = obj->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}

int		irayplane(t_ray *r, t_obj *obj, double *t0, t_env *e)
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
	obj->normal = normalplane;

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
int		irayquadric(t_ray *r, t_obj *obj, double *t0, t_env *e)
{
	double	abcd[4];
	bool	ret;
	t_vec 	cam;

	//r->dir is wrong if there are reflections


	ret = FALSE;
	t_vec camdir = vectorsub(obj->pos, e->cam.eyepoint);
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
//	printf("quad.a = %g\nquad.b = %g\nquad.c = %g\nquad.f = %g\nquad.g = %g\nquad.h = %g\nquad.p = %g\nquad.q = %g\nquad.r = %g\nquad.d = %g\n", quad.a, quad.b, quad.c, quad.f, quad.g, quad.h, quad.p, quad.q, quad.r, quad.d);
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
		if (obj->t[0] > obj->t[1])
		{
			swapdouble(&obj->t[0], &obj->t[1]);
		}

		if (obj->t[0] < 0 && obj->t[1] < 0)
		{
			return(FALSE);
		}
		obj->normal = normalquadric;

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

int		iraycylinder(t_ray *r, t_obj *obj, double *t0, t_env *e)
{
	double	abcd[4];
	t_vec	cam;
	bool	ret;

	ret = FALSE;
	cam = r->dir;
	//printf("cam->dir = %g %g %g\n", cam.x, cam.y, cam.z);
	/*
	abcd[0] = pow(r->start.x, 2) / pow(obj->dir.x, 2);
	printf("abcd[0]1 = %g\n", abcd[0]);
	abcd[0] += pow(r->start.y, 2) / pow(obj->dir.y, 2);
	printf("abcd[0]2 = %g\n", abcd[0]);
	abcd[0] += pow(r->start.z, 2) / pow(obj->dir.z, 2) - pow(obj->rad, 2);
	printf("abcd[0]3 = %g\n", abcd[0]);

	abcd[1] = (2 * r->start.x * r->dir.x) / pow(obj->dir.x, 2);
	abcd[1] += (2 * r->start.y * r->dir.y) / pow(obj->dir.y, 2);
	abcd[1] += (2 * r->start.z * r->dir.z) / pow(obj->dir.z, 2);

	abcd[2] = pow(r->dir.x, 2) / pow(obj->dir.x, 2);
	abcd[2] += pow(r->dir.y, 2) / pow(obj->dir.y, 2);
	abcd[2] += pow(r->dir.z, 2) / pow(obj->dir.z, 2);

	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
*/
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
		obj->t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		obj->t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
	//printf("obj->t[0] = %g\n", obj->t[0]);
		//obj->t[0] = (obj->t[0] < obj->t[1] && obj->t[0] >= 0 ? obj->t[0] : obj->t[1]);

		if (obj->t[0] > obj->t[1])
		{
			swapdouble(&obj->t[0], &obj->t[1]);
		}

		if (obj->t[0] < 0 && obj->t[1] < 0)
		{
			return(FALSE);
		}
		obj->normal = normalcylinder;

		//printf("obj->t[0] = %g\nt[1] = %g\n", obj->t[0], obj->t[1]);
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
	//printf("obj->t[0] = %g\n", obj->t[0]);
		if (/*(obj->t[0] > 0.001f) && */obj->t[0] < *t0)
		{
			*t0 = obj->t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}
