/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:57:51 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 09:40:30 by dmoureu-         ###   ########.fr       */
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
	vectornormalize(&e->n);

	//if the intersection point is located behind the object center from the camera perspective,
	//then we're inside the object
	if (obj->reversen)
	{
		e->n = vectorscale(-1, e->n);
		obj->reversen = FALSE;
	}
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

	if (obj->reversen)
	{
		e->n = vectorscale(-1, e->n);
    obj->reversen = FALSE;
	}
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

	if (obj->reversen)
	{
		e->n = vectorscale(-1, e->n);
		obj->reversen = FALSE;
	}
}

//xn = 2*A*xi + D*yi + E*zi + G
//yn = 2*B*yi + D*xi + F*zi + H
//z n = 2*C*zi + E*xi + F*yi + I
void	normalquadric(t_env *e, t_obj *obj)
{
	t_vec camdir = vectorsub(obj->pos, e->cam.eyepoint);
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

	if (obj->reversen)
	{
		e->n = vectorscale(-1, e->n);
		obj->reversen = FALSE;
	}
	//if the vector from the intersection to the apex is aligned to the cone axis, the it's fine
}
