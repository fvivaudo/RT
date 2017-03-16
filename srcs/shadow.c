/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:57:51 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:34:14 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

//I consider an object like a series of wall instead of a single entity, it might not be the best way to calculate
// transparency, refraction
double		computeshadow(t_env *e, t_ray *r, double light, double dist)
{
	t_obj			*cursor;
	cursor = e->obj;
//	double t = dist; // distance between point and light

	while (cursor)
	{
		if ((cursor->type == TYPE_SPHERE && iraysphere(r, cursor, &dist, e)) ||
			(cursor->type == TYPE_PLANE && irayplane(r, cursor, &dist, e)) ||
			(cursor->type == TYPE_CYLINDER && iraycylinder(r, cursor, &dist, e)) ||
			(cursor->type == TYPE_CONE && iraycone(r, cursor, &dist, e)) ||
			(cursor->type == TYPE_QUADRIC && irayquadric(r, cursor, &dist, e)))
		{
		//	e->t = t;
			light *= cursor->material.transparency; // is it accurate?
		}
		cursor = cursor->nextitem;
	}
	return (light);
	// if an intersection was found
}

/*for each light in the scene
**{
**	if the light is not in shadow of another object
**	{
**		add this light contribution to computed color;
**	}
**}*/
int		deal_shadow(t_env *e)
{
	t_ray			lightray;
	t_light			*tmplight;
	double			tmpdot;
	double			distancetolight;

	tmplight = e->lights;

	while (tmplight)
	{
		if (e->cmat.bump)
		{
			e->n = bump_mapping(e);
		}

		e->clight = *tmplight;
		tmplight = tmplight->next;
		//distance between intersection point and light
		e->dist = vectorsub(e->clight.pos, e->newstart);
		distancetolight = vectormagnitude(e->dist);
//		printf("Checkpoint0\n");
/*		printf("e->newstart.x = %g, e->newstart.y = %g, e->newstart.z = %g\n", e->newstart.x, e->newstart.y, e->newstart.z);
		printf("e->clight.pos.x = %g, e->clight.pos.y = %g, e->clight.pos.z = %g\n", e->clight.pos.x, e->clight.pos.y, e->clight.pos.z);
		printf("e->n.x = %g, e->n.y = %g, e->n.z = %g\n", e->n.x, e->n.y, e->n.z);
		printf("e->dist.x = %g, e->dist.y = %g, e->dist.z = %g\n", e->dist.x, e->dist.y, e->dist.z);
		printf("e->t = %g\n", e->t);*/

		//if the vector dot is negative, then both vectors are going in opposite directions
		vectornormalize(&e->dist);
		if ((tmpdot = vectordot(e->n, e->dist)) <= 0.0f)
			continue;
		e->t = vectormagnitude(e->dist);
		if (e->t <= 0.0f)
			continue; // delete later, eventually

		//Check if there is something between point and light
		//limits light range, or does it?
	//	lightray.dir = vectorscale((1 / e->t), e->dist); //alternative to normalization?
		lightray.dir = e->dist;
		lightray.start = vectoradd(e->newstart, e->dist);
		//lightray.start = vectoradd(e->newstart, lightray.dir);
		// the object risks a colision with itself, which is quite problematic
		// 2 solutions :
		// -forbid colision detection with itself
		// -launch the ray a small distance away from the actual intersection point (CURRENT)
		//printf("intersection\n");
	//	printf("intersection\n");
/*		if (intersection(e, &lightray, -1))
		{
			continue;
		}*/

		double light = 1;
		//temporary removal
		if ((light = computeshadow(e, &lightray, light, distancetolight)) == 0.0) // 1 is light, is it ok?
		{
			continue;
		}
	//	printf("light = %g\n", light);
	//	printf("intersection2\n");

	//	lambert diffusion
		e->lambert = tmpdot * e->coef;
		e->col.red += e->lambert * e->clight.intensity.red * e->cmat.diffuse.red * light;
		e->col.green += e->lambert * e->clight.intensity.green * e->cmat.diffuse.green * light;
		e->col.blue += e->lambert * e->clight.intensity.blue * e->cmat.diffuse.blue * light;

		//check collision with objects between object and light

		// Blinn Phong model
		blinn_phong(e, lightray.dir);
	}
	if (e->col.red == 0 && e->col.green == 0 && e->col.red == 0)
		return (FALSE);
	return (TRUE);
	//Ambient lighting

	//	printf("e->red = %g, e->green = %g, e->blue = %g\n", e->red, e->green, e->blue);
}
