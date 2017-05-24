/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:15 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/01/09 13:39:20 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

t_vec	vectorproduct(t_vec u, t_vec v)
{
	t_vec res;

	res.x = u.y * v.z - u.z * v.y;
	res.y = u.z * v.x - u.x * v.z;
	res.z = u.x * v.y - u.y * v.x;
	return (res);
}

t_vec	vectordiv(t_vec u, t_vec v)
{
	t_vec res;

	res.x = u.y / v.z - u.z / v.y;
	res.y = u.z / v.x - u.x / v.z;
	res.z = u.x / v.y - u.y / v.x;
	return (res);
}

double	vectordot(t_vec v1, t_vec v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

double	vectormagnitude(t_vec v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vec	vectornormalize(t_vec *v)
{
	double mag;

	mag = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
	if (mag)
	{
		v->x /= mag;
		v->y /= mag;
		v->z /= mag;
	}
	else
	{
		*v = vectorinit(0, 0, 0);
	}
	return (*v);
}
