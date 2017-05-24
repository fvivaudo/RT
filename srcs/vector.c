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
