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

/* Rien de tres interessant */

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



void setrotationmatrix(double angle, t_vec axis, double rotmat[4][4]) //normalize entry vector
{
	vectornormalize(&axis);

	//angle = angle * M_PI / 180.0; //converting to radian value
	double x2 = axis.x * axis.x;
	double y2 = axis.y * axis.y;
	double z2 = axis.z * axis.z;
	double cangle = cos(angle);
	double sangle = sin(angle);


	if (cangle < ROUNDING_LIMIT)
		cangle = 0;
	if (sangle < ROUNDING_LIMIT)
		sangle = 0;
//    double rotationMatrix[3][3];

	rotmat[0][0] = (x2 + (y2 + z2) * cangle);
	rotmat[0][1] = (axis.x * axis.y * (1 - cangle) - axis.z * sangle);
	rotmat[0][2] = (axis.x * axis.z * (1 - cangle) + axis.y * sangle);
	rotmat[0][3] = 0;

	rotmat[1][0] = (axis.x * axis.y * (1 - cangle) + axis.z * sangle);
	rotmat[1][1] = (y2 + (x2 + z2) * cangle);
	rotmat[1][2] = (axis.y * axis.z * (1 - cangle) - axis.x * sangle);
	rotmat[1][3] = 0;

	rotmat[2][0] = (axis.x * axis.z * (1 - cangle) - axis.y * sangle);
	rotmat[2][1] = (axis.y * axis.z * (1 - cangle) + axis.x * sangle);
	rotmat[2][2] = (z2 + (x2 + y2) * cangle);
	rotmat[2][3] = 0;

	rotmat[3][0] = 0;
	rotmat[3][1] = 0;
	rotmat[3][2] = 0;
	rotmat[3][3] = 1;

}

void transposematrix(double transposedmatrix[4][4], double rotationmatrix[4][4]) //normalize entry vector
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

void multmatrix(double res[4][4],double m1[4][4], double m2[4][4]) //normalize entry vector
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

//to rotate a quadric, we need to multiply it by a rotation matrix multiplied by it's transposition
t_quadric	quadricrotate(t_quadric to_rot, t_vec r_a, double rad, t_vec pos)
{
	double	tmpquad[4][4];
	double	tmprot[4][4];
	double	tmptranspose[4][4];
	double	tmpres[4][4];
	double	tmp[4][4];

	if (pos.x)
		pos.x = pos.x;

	setrotationmatrix(rad, r_a, tmprot); //tmpquad holds rotation matrix
	transposematrix(tmptranspose, tmprot); //tmprot holds transposed rotation matrix
	//printf("c_rad = %g, r_a.x = %g, tmp_rad = %g\n", c_rad, r_a.x, tmp_rad);

	printf("rotation matrix\n");
	for (int c = 0; c < 4; ++c)
	{
		for (int d = 0; d < 4; ++d)
		{
			printf("%10g\t", tmprot[c][d]);
		}
		printf("\n");
	}

	tmpquad[0][0] = to_rot.a;
	tmpquad[0][1] = to_rot.h;
	tmpquad[0][2] = to_rot.g;
	tmpquad[1][0] = to_rot.h;
	tmpquad[1][1] = to_rot.b;
	tmpquad[1][2] = to_rot.f;
	tmpquad[2][0] = to_rot.g;
	tmpquad[2][1] = to_rot.f;
	tmpquad[2][2] = to_rot.c;

	tmpquad[0][3] = to_rot.p;
	tmpquad[1][3] = to_rot.f;
	tmpquad[2][3] = to_rot.c;

	tmpquad[3][0] = to_rot.g;
	tmpquad[3][1] = to_rot.f;
	tmpquad[3][2] = to_rot.c;
	tmpquad[3][3] = to_rot.d;

	printf("input matrix\n");
	for (int c = 0; c < 4; ++c)
	{
		for (int d = 0; d < 4; ++d)
		{
			printf("%10g\t", tmpquad[c][d]);
		}
		printf("\n");
	}

	multmatrix(tmp, tmptranspose, tmpquad);

	printf("mult1\n");
	for (int c = 0; c < 4; ++c)
	{
		for (int d = 0; d < 4; ++d)
		{
			printf("%10g\t", tmp[c][d]);
		}
		printf("\n");
	}

	multmatrix(tmpres, tmp, tmprot);


/*
	for(int i = 0; i < 3; i++ )
	{
		for(int j = 0; j < 3; j++)
		{
			tmpres[i][j] = 0;
			for(int k = 0; k < 3; k++)
			{
				tmpres[i][j] += tmprot[i][k] * tmpquad[k][j];
			}
		}
	}*/

	to_rot.a = tmpres[0][0];
	to_rot.h = tmpres[0][1];
	to_rot.g = tmpres[0][2];
	to_rot.h = tmpres[1][0];
	to_rot.b = tmpres[1][1];
	to_rot.f = tmpres[1][2];
	to_rot.g = tmpres[2][0];
	to_rot.f = tmpres[2][1];
	to_rot.c = tmpres[2][2];

	to_rot.p = tmpres[0][3];
	to_rot.f = tmpres[1][3];
	to_rot.c = tmpres[2][3];
	to_rot.g = tmpres[3][0];
	to_rot.f = tmpres[3][1];
	to_rot.c = tmpres[3][2];
	to_rot.d = tmpres[3][3];

	printf("rotated matrix\n");
	for (int c = 0; c < 4; ++c)
	{
		for (int d = 0; d < 4; ++d)
		{
			printf("%10g\t", tmpres[c][d]);
		}
		printf("\n");
	}
	return (to_rot);
}
/*
t_quadric	quadricrotate(t_quadric to_rot, t_vec r_a, double rad)
{
	double	tmpquad[3][3];
	double	tmprot[3][3];
	double	tmpres[3][3];
	double 	c_rad;
	double 	s_rad;
	double	tmp_rad;
	double	sum;

	c_rad = cos(rad);
	s_rad = sin(rad);

	if (c_rad < ROUNDING_LIMIT)
		c_rad = 0;
	if (s_rad < ROUNDING_LIMIT)
		s_rad = 0;

	tmp_rad = 1.0 - c_rad;

	//printf("c_rad = %g, r_a.x = %g, tmp_rad = %g\n", c_rad, r_a.x, tmp_rad);
	tmpres[0][0] = (c_rad + r_a.x * r_a.x * (tmp_rad));
	tmpres[0][1] = (r_a.x * r_a.y * (tmp_rad) - r_a.z * s_rad);
	tmpres[0][2] = (r_a.x * r_a.z * (tmp_rad)) + r_a.y * s_rad;
	tmpres[1][0] = (r_a.y * r_a.x * (tmp_rad) + r_a.z * s_rad);
	tmpres[1][1] = (c_rad + r_a.y * r_a.y * (tmp_rad));
	tmpres[1][2] = (r_a.y * r_a.z * (tmp_rad) - r_a.x * s_rad);
	tmpres[2][0] = (r_a.z * r_a.x * (tmp_rad) - r_a.y * s_rad);
	tmpres[2][1] = (r_a.z * r_a.y * (tmp_rad) + r_a.x * s_rad);
	tmpres[2][2] = (c_rad + r_a.z * r_a.z * (tmp_rad)); //tmprot


	tmpquad[0][0] = to_rot.a;
	tmpquad[0][1] = to_rot.h;
	tmpquad[0][2] = to_rot.g;
	tmpquad[1][0] = to_rot.h;
	tmpquad[1][1] = to_rot.b;
	tmpquad[1][2] = to_rot.f;
	tmpquad[2][0] = to_rot.g;
	tmpquad[2][1] = to_rot.f;
	tmpquad[2][2] = to_rot.c;

	int c;
	int d;
	int k;

//	printf("initital matrix\n");
	for (c = 0; c < 3; ++c)
	{
		for (d = 0; d < 3; ++d)
		{
//			printf("%10g\t", tmpquad[c][d]);
		}
//		printf("\n");
	}


	int i;
	int j;
	double determinant = 0;

	for(i = 0; i < 3; i++)
	{
		for(j = 3; j < 2*3; j++)
		{

			if(i == (j - 3))
			{
				matrix[i][j] = 1.0;
			}
			else
			{
				matrix[i][j] = 0.0;
			}

		}

	}

	for(i = 0; i < 3; i++)
	{

		for(j = 0; j < 3; j++)
		{

			if(i != j)
			{
				ratio = matrix[j][i]/matrix[i][i];

				for(k = 0; k < 2 * 3; k++)
				{
					matrix[j][k] -= ratio * matrix[i][k];

				}

			}

		}

	}

	for(i = 0; i < 3; i++)
	{

		a = matrix[i][i];

		for(j = 0; j < 2*3; j++)
		{
			matrix[i][j] /= a;

		}

	}

	printf("rotation matrix\n");
	for (c = 0; c < 3; ++c)
	{
		for (d = 0; d < 3; ++d)
		{
	//		printf("%10g\t", tmprot[c][d]);
		}
	//	printf("\n");
	}

	for (c = 0; c < 3; ++c)
	{
		for (d = 0; d < 3; ++d)
		{
			sum = 0;
			for (k = 0; k < 3; ++k)
			{
				sum += tmpquad[c][k] * tmprot[k][d];
			}
			tmpres[c][d] = fabs(sum) > ROUNDING_LIMIT ? sum : 0;
		}
	}

	to_rot.a = tmpres[0][0];
	to_rot.h = tmpres[0][1];
	to_rot.g = tmpres[0][2];
	to_rot.h = tmpres[1][0];
	to_rot.b = tmpres[1][1];
	to_rot.f = tmpres[1][2];
	to_rot.g = tmpres[2][0];
	to_rot.f = tmpres[2][1];
	to_rot.c = tmpres[2][2];
	printf("rotated matrix\n");
	for (c = 0; c < 3; ++c)
	{
		for (d = 0; d < 3; ++d)
		{
	//		printf("%10g\t", tmpres[c][d]);
		}
	//	printf("\n");
	}
	return (to_rot);
}*/
/*
t_vec		vectorrotate(t_vec v, t_vec axis, double angle)
{
	double		cs_angle[2];
	t_vec	v_t_1;
	t_vec	v_t_2;
	t_vec	v_t_3;
	t_vec	r_v;

	cs_angle[0] = cos(-angle);
	cs_angle[1] = sin(-angle);
	v_t_1 = vectorscale((1 - cs_angle[0]), axis);
	v_t_3 = vectorscale(vectordot(v, v_t_1), axis);
	v_t_1 = vectorinit(0, 0, 0);
	v_t_2 = vectorscale(cs_angle[1], axis);
	v_t_1 = vectorproduct(v, v_t_1);
	v_t_2 = vectorscale(cs_angle[0], v);
	r_v = vectoradd(v_t_1, vectoradd(v_t_2, v_t_3));
	return (r_v);
}*/

