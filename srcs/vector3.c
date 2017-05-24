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
