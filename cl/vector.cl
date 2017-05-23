#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#include <RTopencl.h>

void	swapdouble(double *a, double *b)
{
	double c;

	c = *a;
	*a = *b;
	*b = c;
}

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


/*
** Rotate the vector v and return it as a fresh new copy
*/
//multiply by initital vector by rotation matrix
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

//http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
//rotating the point (x,y,z) about the line through (a,b,c) with direction vector ⟨u,v,w⟩ (where u2 + v2 + w2 = 1) by the angle θ.

t_vec vectorpointrotatearoundaxis(t_vec axp, t_vec axd, t_vec p, double theta) 
{
	// Set some intermediate values.
	double u2 = axd.x * axd.x;
	double v2 = axd.y * axd.y;
	double w2 = axd.z * axd.z;
	double cost = cos(theta);
	double omc = 1 - cost;
	double sint = sin(theta);

	// Use the formula in the paper.
	t_vec res;
	res.x = ((axp.x *(v2 + w2) - axd.x*(axp.y*axd.y + axp.z*axd.z - axd.x*p.x - axd.y*p.y - axd.z*p.z)) * omc
			+ p.x*cost
			+ (-axp.z*axd.y + axp.y*axd.z - axd.z*p.y + axd.y*p.z)*sint);

	res.y = ((axp.y*(u2 + w2) - axd.y*(axp.x*axd.x + axp.z*axd.z - axd.x*p.x - axd.y*p.y - axd.z*p.z)) * omc
			+ p.y*cost
			+ (axp.z*axd.x - axp.x*axd.z + axd.z*p.x - axd.x*p.z)*sint);

	res.z = ((axp.z*(u2 + v2) - axd.z*(axp.x*axd.x + axp.y*axd.y - axd.x*p.x - axd.y*p.y - axd.z*p.z)) * omc
			+ p.z*cost
			+ (-axp.y*axd.x + axp.x*axd.y - axd.y*p.x + axd.x*p.y)*sint);

	return (res);
}