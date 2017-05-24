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

t_vec		vectorinit(double x, double y, double z)
{
	t_vec vec;

	vec.x = x;
	vec.y = y;
	vec.z = z;
	return (vec);
}

t_vec	vectorscale(double c, t_vec v)
{
	t_vec res;

	res.x = v.x * c;
	res.y = v.y * c;
	res.z = v.z * c;
	return (res);
}

t_vec	vectorscalediv(double c, t_vec v)
{
	t_vec res;

	res.x = v.x / c;
	res.y = v.y / c;
	res.z = v.z / c;
	return (res);
}

t_vec	vectoradd(t_vec v1, t_vec v2)
{
	t_vec res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

t_vec	vectorsub(t_vec v1, t_vec v2)
{
	t_vec res;

	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return (res);
}

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

t_vec	vectorrotate(t_vec to_rot, t_vec r_a, double rad)
{
	t_vec	res;
	double 	c_rad;
	double 	s_rad;
	double	tmp_rad;

	c_rad = cos(rad);
	s_rad = sin(rad);
	tmp_rad = 1 - c_rad;
	res.x = to_rot.x * (c_rad + r_a.x * r_a.x * (tmp_rad));
	res.x += to_rot.y * (r_a.x * r_a.y * (tmp_rad) - r_a.z * s_rad);
	res.x += to_rot.z * (r_a.x * r_a.z * (tmp_rad)) + r_a.y * s_rad;
	res.y = to_rot.x * (r_a.y * r_a.x * (tmp_rad) + r_a.z * s_rad);
	res.y += to_rot.y * (c_rad + r_a.y * r_a.y * (tmp_rad));
	res.y += to_rot.z * (r_a.y * r_a.z * (tmp_rad) - r_a.x * s_rad);
	res.z = to_rot.x * (r_a.z * r_a.x * (tmp_rad) - r_a.y * s_rad);
	res.z += to_rot.y * (r_a.z * r_a.y * (tmp_rad) + r_a.x * s_rad);
	res.z += to_rot.z * (c_rad + r_a.z * r_a.z * (tmp_rad));
	return (res);
}

typedef struct s_vpr
{
	double u2 ;
	double v2 ;
	double w2 ;
	double cost;
	double omc ;
	double sint;
	t_vec res;
}				t_vpr;

t_vec vectorpointrotatearoundaxis(t_vec axp, t_vec axd, t_vec p, double theta)
{
	t_vpr u;

	u.u2 = axd.x * axd.x;
	u.v2 = axd.y * axd.y;
	u.w2 = axd.z * axd.z;
	u.cost = cos(theta);
	u.omc = 1 - u.cost;
	u.sint = sin(theta);
	u.res.x = ((axp.x *(u.v2 + u.w2) - axd.x*(axp.y*axd.y +
		axp.z*axd.z - axd.x*p.x - axd.y*p.y - axd.z*p.z)) * u.omc
			+ p.x*u.cost
			+ (-axp.z*axd.y + axp.y*axd.z - axd.z*p.y + axd.y*p.z)*u.sint);
	u.res.y = ((axp.y*(u.u2 + u.w2) - axd.y*(axp.x*axd.x
		+ axp.z*axd.z - axd.x*p.x - axd.y*p.y - axd.z*p.z)) * u.omc
			+ p.y*u.cost
			+ (axp.z*axd.x - axp.x*axd.z + axd.z*p.x - axd.x*p.z)*u.sint);
	u.res.z = ((axp.z*(u.u2 + u.v2) - axd.z*(axp.x*axd.x
		+ axp.y*axd.y - axd.x*p.x - axd.y*p.y - axd.z*p.z)) * u.omc
			+ p.z*u.cost
			+ (-axp.y*axd.x + axp.x*axd.y - axd.y*p.x + axd.x*p.y)*u.sint);
	return (u.res);
}

void setrotationmatrix_func(double rotmat[4][4])
{
	rotmat[2][3] = 0;
	rotmat[3][0] = 0;
	rotmat[3][1] = 0;
	rotmat[3][2] = 0;
	rotmat[3][3] = 1;
}

typedef struct s_srtm
{
	double x2 ;
	double y2 ;
	double z2 ;
	double cangle ;
	double sangle ;
}				t_srtm;

t_srtm 	setrotationmatrix_init(double angle, t_vec axis)
{
	t_srtm u;

	u.x2 = axis.x * axis.x;
	u.y2 = axis.y * axis.y;
	u.z2 = axis.z * axis.z;
	u.cangle = cos(angle);
	u.sangle = sin(angle);
	return(u);
}

void setrotationmatrix(double angle, t_vec axis, double rotmat[4][4])
{
	t_srtm u;

	vectornormalize(&axis);
	u = setrotationmatrix_init(angle, axis);
	if (u.cangle < ROUNDING_LIMIT)
		u.cangle = 0;
	if (u.sangle < ROUNDING_LIMIT)
		u.sangle = 0;
	rotmat[0][0] = (u.x2 + (u.y2 + u.z2) * u.cangle);
	rotmat[0][1] = (axis.x * axis.y * (1 - u.cangle) - axis.z * u.sangle);
	rotmat[0][2] = (axis.x * axis.z * (1 - u.cangle) + axis.y * u.sangle);
	rotmat[0][3] = 0;
	rotmat[1][0] = (axis.x * axis.y * (1 - u.cangle) + axis.z * u.sangle);
	rotmat[1][1] = (u.y2 + (u.x2 + u.z2) * u.cangle);
	rotmat[1][2] = (axis.y * axis.z * (1 - u.cangle) - axis.x * u.sangle);
	rotmat[1][3] = 0;
	rotmat[2][0] = (axis.x * axis.z * (1 - u.cangle) - axis.y * u.sangle);
	rotmat[2][1] = (axis.y * axis.z * (1 - u.cangle) + axis.x * u.sangle);
	rotmat[2][2] = (u.z2 + (u.x2 + u.y2) * u.cangle);
	setrotationmatrix_func(rotmat);
}

void transposematrix(double transposedmatrix[4][4], double rotationmatrix[4][4])
{
	int i;
	int y;

	i = 0;
	y = 0;
	while (i < 4)
	{
		while (y < 4)
		{
			transposedmatrix[i][y] = rotationmatrix[y][i];
			++y;
		}
		y = 0;
		++i;
	}
}

void multmatrix(double res[4][4],double m1[4][4], double m2[4][4])
{
	int i;
	int y;
	int z;

	i = 0;
	y = 0;
	z = 0;
	while (i < 4)
	{
		while (y < 4)
		{
			res[i][y] = 0;
			while (z < 4)
			{
				res[i][y] += m1[i][z] * m2[z][y];
				++z;
			}
			z = 0;
			++y;
		}
		y = 0;
		++i;
	}
}

typedef struct s_qrte
{
	double	tmpquad[4][4];
	double	tmprot[4][4];
	double	tmptranspose[4][4];
	double	tmpres[4][4];
	double	tmp[4][4];
}				t_qrte;

void quadricrotate_func(t_qrte *u, t_quadric *to_rot)
{
	to_rot->a = u->tmpres[0][0];
	to_rot->h = u->tmpres[0][1];
	to_rot->g = u->tmpres[0][2];
	to_rot->h = u->tmpres[1][0];
	to_rot->b = u->tmpres[1][1];
	to_rot->f = u->tmpres[1][2];
	to_rot->g = u->tmpres[2][0];
	to_rot->f = u->tmpres[2][1];
	to_rot->c = u->tmpres[2][2];
	to_rot->p = u->tmpres[0][3];
	to_rot->f = u->tmpres[1][3];
	to_rot->c = u->tmpres[2][3];
	to_rot->g = u->tmpres[3][0];
	to_rot->f = u->tmpres[3][1];
	to_rot->c = u->tmpres[3][2];
	to_rot->d = u->tmpres[3][3];
}

void quadricrotate_func2(t_qrte *u, t_quadric *to_rot)
{
	u->tmpquad[0][0] = to_rot->a;
	u->tmpquad[0][1] = to_rot->h;
	u->tmpquad[0][2] = to_rot->g;
	u->tmpquad[1][0] = to_rot->h;
	u->tmpquad[1][1] = to_rot->b;
	u->tmpquad[1][2] = to_rot->f;
	u->tmpquad[2][0] = to_rot->g;
	u->tmpquad[2][1] = to_rot->f;
	u->tmpquad[2][2] = to_rot->c;
	u->tmpquad[0][3] = to_rot->p;
	u->tmpquad[1][3] = to_rot->f;
	u->tmpquad[2][3] = to_rot->c;
	u->tmpquad[3][0] = to_rot->g;
	u->tmpquad[3][1] = to_rot->f;
	u->tmpquad[3][2] = to_rot->c;
	u->tmpquad[3][3] = to_rot->d;
}

t_quadric	quadricrotate(t_quadric to_rot, t_vec r_a, double rad, t_vec pos)
{
	t_qrte u;

	if (pos.x)
		pos.x = pos.x;
	setrotationmatrix(rad, r_a, u.tmprot);
	transposematrix(u.tmptranspose, u.tmprot);
	quadricrotate_func2(&u, &to_rot);
	multmatrix(u.tmp, u.tmptranspose, u.tmpquad);
	multmatrix(u.tmpres, u.tmp, u.tmprot);
	quadricrotate_func(&u, &to_rot);
	return (to_rot);
}
