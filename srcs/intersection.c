/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:58:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2016/12/21 16:48:51 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>


void	normalplane(t_env *e, t_obj *obj)
{
	//vector dot > 0 check if both vectors are going in the same direction
	//if they are not, get the reverse direction of the plane to get it's normal
	if (vectordot(obj->dir, e->r.dir) >= 0.0)
	{
		e->n = vectorscale(-1, obj->dir);
	}
	else
	{
		e->n = obj->dir;
	}
}

void	normalsphere(t_env *e, t_obj *obj)
{
		//calcul de la normale d'une sphere
		//get distance between new distant ray and object pos?
		// distance = root of  (v1 - v2).(v1 - v2)
		e->n = vectorsub(e->newstart, obj->pos);
		e->temp = vectordot(e->n, e->n);
		if (e->temp == 0)
			return ;
		//the root of the dot product
		e->temp = 1.0f / sqrtf(e->temp);
		e->n = vectorscale(e->temp, e->n);
}

double	getdegree(double rad)
{
	return (rad * 180 / M_PI);
}

void	normalcylinder(t_env *e, t_obj *obj)
{
	t_vec	y_axis;
	t_vec	rot_axis;
	//t_vec	dir_cyl;
	t_vec	center_cyl;
	t_vec	inter_point;
	//t_vec	pos_cyl;
	double	rot_angle;

	y_axis = vectorinit(0, 1, 0);
	rot_angle = acos(vectordot(obj->dir, y_axis));
	//get rotation angle

	rot_axis = vectorproduct(obj->dir, y_axis);
	//get orthogonal rotation vector

	vectornormalize(&rot_axis);

	if (rot_axis.x || rot_axis.y || rot_axis.z)
	{
		inter_point = vectorrotate(e->newstart, rot_axis, rot_angle);
	}
	else
	{
		inter_point = e->newstart;
	}

	//get a point in the center of the cylinder at the same level as the intersection
	center_cyl = vectorinit(obj->pos.x, inter_point.y, obj->pos.z);
	//getting vector from center of cylinder slice toward the intersection point
	e->n = vectorsub(inter_point, center_cyl);
	if (rot_axis.x || rot_axis.y || rot_axis.z)
	{
		e->n = vectorrotate(e->n, rot_axis, rot_angle);
	}

	vectornormalize(&e->n);
}

//x = -r cos(rad)
//y = r
//z = -r sin(rad)
//n = e->newstart - magnitude(newstart - cone.pos) / cos(e->alpha) * cone.dir
void	normalcone(t_env *e, t_obj *obj)
{
	double tmp;

	//if the vector from the intersection to the apex is aligned to the cone axis, the it's fine
	if (vectordot(vectorsub(e->newstart, obj->pos), obj->dir) > 0)
		tmp = vectormagnitude(vectorsub(e->newstart, obj->pos)) / cos(obj->alpha);
	else
		tmp = vectormagnitude(vectorsub(e->newstart, obj->pos)) / cos(obj->alpha) * (-1);

	//Removing object pos from the formula make it only true for a cone at the origin
	e->n = vectorsub(e->newstart, vectoradd(vectorscale(tmp, obj->dir), obj->pos));
	vectornormalize(&e->n);
}



t_quadric initquad(double param[10])
{
	t_quadric quad;

	quad.a = param[0];
	quad.b = param[1];
	quad.c = param[2];
	quad.f = param[3];
	quad.g = param[4];
	quad.h = param[5];
	quad.p = param[6];
	quad.q = param[7];
	quad.r = param[8];
	quad.d = param[9];

	return (quad);
}
//xn = 2*A*xi + D*yi + E*zi + G
//yn = 2*B*yi + D*xi + F*zi + H
//z n = 2*C*zi + E*xi + F*yi + I
void	normalquadric(t_env *e, t_obj *obj, t_vec eyepoint)
{
	t_vec camdir = vectorsub(obj->pos, eyepoint);
	double tmpdist = vectormagnitude(camdir);
	vectornormalize(&camdir);

//	t_vec tmp_start = vectorscale(-1, vectorscale(tmpdist, camdir));
	t_vec tmp_start = vectoradd(e->newstart, vectorscale(-1, vectorscale(tmpdist + SCREEN_EYE_DIST, camdir)));

	e->n.x = 2 * obj->quad.a * tmp_start.x;
	e->n.x += 2 * obj->quad.f * tmp_start.y;
	e->n.x += 2 * obj->quad.g * tmp_start.z;
	e->n.x += 2 * obj->quad.p;
	e->n.y = 2 * obj->quad.b * tmp_start.y;
	e->n.y += 2 * obj->quad.f * tmp_start.x;
	e->n.y += 2 * obj->quad.h * tmp_start.z;
	e->n.y += 2 * obj->quad.q;
	e->n.z = 2 * obj->quad.c * tmp_start.z;
	e->n.z += 2 * obj->quad.g * tmp_start.x;
	e->n.z += 2 * obj->quad.h * tmp_start.y;
	e->n.z += 2 * obj->quad.r;
	vectornormalize(&e->n);
	//printf("%s\n", );
	//printf("normal e->r.start.x = %g, e->r.start.y = %g, e->r.start.z = %g\n", e->r.start.x, e->r.start.y, e->r.start.z);
//	printf("tmp_start.x = %g, tmp_start.y = %g, tmp_start.z = %g\n", tmp_start.x, tmp_start.y, tmp_start.z);
//	printf("e->n.x = %g, e->n.y = %g, e->n.z = %g\n", e->n.x, e->n.y, e->n.z);
	if (vectordot(e->n, e->r.dir) >= 0)
	{
		//e->n = vectorscale(-1, e->n);
	}
	//if the vector from the intersection to the apex is aligned to the cone axis, the it's fine
}

//I consider an object like a series of wall instead of a single entity, it might not be the best way to calculate
// transparency, refraction
double		computeshadow(t_env *e, t_ray *r, double light, double dist)
{
	t_obj			*cursor;
	cursor = e->obj;
//	double t = dist; // distance between point and light

	while (cursor)
	{
		if ((cursor->type == TYPE_SPHERE && iraysphere(r, cursor, &dist)) ||
			(cursor->type == TYPE_PLANE && irayplane(r, cursor, &dist)) ||
			(cursor->type == TYPE_CYLINDER && iraycylinder(r, cursor, &dist)) ||
			(cursor->type == TYPE_CONE && iraycone(r, cursor, &dist)) ||
			(cursor->type == TYPE_QUADRIC && irayquadric(r, cursor, &dist, e->cam.eyepoint)))
		{
		//	e->t = t;
			light *= cursor->material.transparency; // is it accurate?
		}
		cursor = cursor->next;
	}
	return (light);
	// if an intersection was found
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
		//	cursor = cursor->next;
		//	continue;
		}
		if ((cursor->type == TYPE_SPHERE && iraysphere(r, cursor, &t)) ||
			(cursor->type == TYPE_PLANE && irayplane(r, cursor, &t)) ||
			(cursor->type == TYPE_CYLINDER && iraycylinder(r, cursor, &t)) ||
			(cursor->type == TYPE_CONE && iraycone(r, cursor, &t)) ||
			(cursor->type == TYPE_QUADRIC && irayquadric(r, cursor, &t, e->cam.eyepoint)))
		{
			e->t = t;
			res = cursor;
		}
		cursor = cursor->next;
	}
	return (res);
	// if an intersection was found
}

t_obj	*computeray(t_env *e)
{
	t_obj *res;

	//printf("slowdown\n");
	/*for(int i = 0; i < 100000 ; ++i)
	{
		int j = i+i;
		(void)j;
	}*/
	if ((res = intersection(e, &e->r, -1)))
	{
		e->id = res->id;
		//e->scaled = distance between start of ray and object intersection
		e->scaled = vectorscale(e->t, e->r.dir);
		/*printf("e->r.dir.x = %g, e->r.dir.y = %g, e->r.dir.z = %g\n", e->r.dir.x, e->r.dir.y, e->r.dir.z);
		printf("e->scaled.x = %g, e->scaled.y = %g, e->scaled.z = %g\n", e->scaled.x, e->scaled.y, e->scaled.z);
		printf("e->t = %g\n", e->t);*/
		//e->newstart = object intersection
		e->newstart = vectoradd(e->r.start, e->scaled);

		//printf("e->newstart.x = %g, e->newstart.y = %g, e->newstart.z = %g\n", e->newstart.x, e->newstart.y, e->newstart.z);

		if ((res->type) == TYPE_PLANE)
			normalplane(e, res);
		else if ((res->type) == TYPE_CYLINDER)
			normalcylinder(e, res);
		else if ((res->type) == TYPE_CONE)
			normalcone(e, res);
		else if ((res->type) == TYPE_SPHERE)
			normalsphere(e, res);
		else if ((res->type) == TYPE_QUADRIC)
			normalquadric(e, res, e->cam.eyepoint);
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

int		iraycone(t_ray *r, t_obj *co, double *t0)
{
	t_vec	delt_p;
	t_vec	tmp[3];
	double	abcd[4];
	double	t[2];

	delt_p = vectorinit(r->start.x - co->pos.x, r->start.y - co->pos.y,
	r->start.z - co->pos.z);
	tmp[0] = vectorscale(vectordot(r->dir, co->dir), co->dir);
	tmp[1] = vectorsub(r->dir, tmp[0]);
	tmp[0] = vectorscale(vectordot(delt_p, co->dir), co->dir);
	tmp[2] = vectorsub(delt_p, tmp[0]);
	abcd[0] = (pow(cos(co->alpha), 2)
	* vectordot(tmp[1], tmp[1])) - (pow(sin(co->alpha), 2)
	* pow(vectordot(r->dir, co->dir), 2));
	abcd[1] = 2 * ((pow(cos(co->alpha), 2)
	* vectordot(tmp[1], tmp[2])) - (pow(sin(co->alpha), 2)
	* vectordot(r->dir, co->dir) * vectordot(delt_p, co->dir)));
	abcd[2] = (pow(cos(co->alpha), 2)
	* vectordot(tmp[2], tmp[2])) - (pow(sin(co->alpha), 2)
	* pow(vectordot(delt_p, co->dir), 2));
	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
	return (iraycone2(abcd, t, t0));
}

int		irayplane(t_ray *r, t_obj *p, double *t0)
{
	t_vec	tmp;
	double	tmpt;

	tmp = vectorsub(p->pos, r->start);
	if (vectordot(p->dir, r->dir) != 0)
		tmpt = vectordot(p->dir, tmp) / vectordot(p->dir, r->dir);
	else
	{
		return (FALSE);
	}
	if (tmpt > 0 && tmpt < *t0)
	{
		*t0 = tmpt;
		return (TRUE);
	}
	return (FALSE);
}

//Aq = Axd2 + Byd2 + Czd2 + Dxdyd + Exdzd + Fydzd
//Bq = 2*Axoxd + 2*Byoyd + 2*Czozd + D(xoyd + yoxd) + E(xozd + zoxd) + F(yozd + ydzo) + Gxd + Hyd + Izd
//Cq = Axo2 + Byo2 + Czo2 + Dxoyo + Exozo + Fyozo + Gxo + Hyo + Izo + J
int		irayquadric(t_ray *r, t_obj *obj, double *t0, t_vec eyepoint)
{
	double	abcd[4];
	double	t[2];

	t_vec cam;

	//r->dir is wrong if there are reflections

	t_vec camdir = vectorsub(obj->pos, eyepoint);
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
		t[0] = -abcd[2] / abcd[1];
		if (t[0] > 0 && t[0] < *t0)
		{
			*t0 = t[0];
			return (TRUE);
		}
		return (FALSE);
	}
	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
	if (abcd[3] >= 0)
	{
		t[0] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
	//	if (t[0] <= 0)
			t[1] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
	//	else
	//		t[1] = MAX_RANGE + 1;
		t[0] = (t[0] < t[1] && t[0] >= 0 ? t[0] : t[1]);
		if (t[0] > 0 && t[0] < *t0)
		{
			*t0 = t[0];
//			printf("t0 = %g\n", *t0);
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
int		iraycylinder(t_ray *r, t_obj *cyl, double *t0)
{
	double	abcd[4];
	double	t[2];
	t_vec cam;

	cam = r->dir;
	//printf("cam->dir = %g %g %g\n", cam.x, cam.y, cam.z);
	abcd[0] = pow(r->start.x, 2) / pow(cyl->dir.x, 2);
	abcd[0] += pow(r->start.y, 2) / pow(cyl->dir.y, 2);
	abcd[0] += pow(r->start.z, 2) / pow(cyl->dir.z, 2) - pow(cyl->rad, 2);

	abcd[1] = (2 * r->start.x * r->dir.x) / pow(cyl->dir.x, 2);
	abcd[1] += (2 * r->start.y * r->dir.y) / pow(cyl->dir.y, 2);
	abcd[1] += (2 * r->start.z * r->dir.z) / pow(cyl->dir.z, 2);

	abcd[2] = pow(r->dir.x, 2) / pow(cyl->dir.x, 2);
	abcd[2] += pow(r->dir.y, 2) / pow(cyl->dir.y, 2);
	abcd[2] += pow(r->dir.z, 2) / pow(cyl->dir.z, 2);

	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
	if (abcd[3] >= 0)
	{
		t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
		t[0] = (t[0] < t[1] && t[0] >= 0 ? t[0] : t[1]);
		if (t[0] > 0 && t[0] < *t0)
		{
			*t0 = t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}