/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:57:59 by fvivaudo          #+#    #+#             */
/*   Updated: 2016/11/25 12:48:56 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

void		init_cam(t_env *e, char **buffer)
{
	t_vec	up;
	t_vec	u;
	t_vec	v;

	t_vec	tmp_up;
	t_vec	tmp_vdir;
	t_vec	rot_axis;
	double	rot_angle;
	if (buffer[1] && buffer[2] && buffer[3])
		e->cam.eyepoint = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return;

	if (!ft_strcmp("LOOKAT", buffer[4]) && buffer[5] && buffer[6] && buffer[7])
	{
		e->cam.lookat = vectorinit(ft_datoi(buffer[5]), ft_datoi(buffer[6]), ft_datoi(buffer[7]));
		e->cam.vdir = vectorsub(e->cam.lookat, e->cam.eyepoint);
		vectornormalize(&e->cam.vdir);
		e->cam.lookat = vectorscale(SCREEN_EYE_DIST, e->cam.vdir); //temporary I hope
	}
	else if (!ft_strcmp("ORIENTATION", buffer[4]) && buffer[5] && buffer[6] && buffer[7])
	{
		e->cam.vdir = vectorinit(ft_datoi(buffer[5]), ft_datoi(buffer[6]), ft_datoi(buffer[7]));
		vectornormalize(&e->cam.vdir);
		e->cam.lookat = vectorscale(SCREEN_EYE_DIST, e->cam.vdir); //temporary I hope
	}
	else
		return;
/*	if (buffer)
	{
	e->cam.lookat = vectorinit(0, 0, -900);
	e->cam.eyepoint = vectorinit(0, 0, -1000);
	}*/

	//printf("e->cam.vdir.x = %g, e->cam.vdir.y = %g, e->cam.vdir.z = %g\n", e->cam.vdir.x, e->cam.vdir.y, e->cam.vdir.z);
	
	//the tmp value represent an ordinary camera, that we'll rotate to fit the actual camera position and direction
	tmp_vdir = vectorinit(0, 0, 1);
	tmp_up = vectorinit(0, -1, 0); // why -1? Dunno, it works for now.

	rot_angle = acos(vectordot(tmp_vdir,  e->cam.vdir));
	rot_axis = vectorproduct(tmp_vdir,  e->cam.vdir);

	vectornormalize(&rot_axis);

	if (rot_axis.x || rot_axis.y || rot_axis.z)
	{
		up = vectorrotate(tmp_up, rot_axis, rot_angle);
	}
	else
	{
		up = tmp_up;
	}
//	rot_axis = vectorproduct(e->cam.vdir, up);
	//up = vectorrotate(e->cam.vdir, rot_axis, 90);
	//vectornormalize(&up);
//	printf("up.x = %g, up.y = %g, up.z = %g\n", up.x, up.y, up.z);
	u = vectorproduct(e->cam.vdir, up);
	v = vectorproduct(u, e->cam.vdir);

	u = vectornormalize(&u);
	v = vectornormalize(&v);

//viewPlaneUpLeft = camPos + ((vecDir*viewplaneDist)+(upVec*(viewplaneHeight/2.0f))) - (rightVec*(viewplaneWidth/2.0f))
//	e->cam.viewplanebottomleftpoint = vectorsub(
//	vectorsub(e->cam.lookat, vectorscale(HEIGHT / 2, v)), vectorscale(WIDTH / 2, u));
	e->cam.viewplanebottomleftpoint = vectorsub(
	vectorsub(e->cam.lookat, vectorscale(HEIGHT / 2, v)), vectorscale(WIDTH / 2, u));
	e->cam.viewplanebottomleftpoint = vectoradd (e->cam.viewplanebottomleftpoint, e->cam.eyepoint);
	e->cam.viewplanebottomleftpoint = vectoradd (e->cam.viewplanebottomleftpoint, e->cam.lookat);
	t_vec tmp = vectoradd(vectorscale(SCREEN_EYE_DIST, e->cam.vdir), e->cam.eyepoint);
//	printf("e->cam.lookat.x = %g, e->cam.lookat.y = %g, e->cam.lookat.z = %g\n", e->cam.lookat.x, e->cam.lookat.y, e->cam.lookat.z);
//	printf("tmp.x = %g, tmp.y = %g, tmp.z = %g\n", tmp.x, tmp.y, tmp.z);
	/*e->cam.viewplanebottomleftpoint = vectorsub(
	vectorsub(vectoradd(vectorscale(200, e->cam.vdir), e->cam.eyepoint), vectorscale(HEIGHT / 2, v)),
	vectorscale(WIDTH / 2, u));*/ //200 is arbitrary
//	e->cam.viewplanebottomleftpoint = vectorsub(vectorscale(HEIGHT / 2, v),
//	vectorscale(WIDTH / 2, u));
//	printf("e->cam.viewplanebottomleftpoint.x = %g, e->cam.viewplanebottomleftpoint.y = %g, e->cam.viewplanebottomleftpoint.z = %g\n", e->cam.viewplanebottomleftpoint.x, e->cam.viewplanebottomleftpoint.y, e->cam.viewplanebottomleftpoint.z);
	e->cam.xincvector = vectorscale(1 / (double)WIDTH,
	(vectorscale(2 * WIDTH / 2, u)));
	e->cam.yincvector = vectorscale(1 / (double)HEIGHT,
	(vectorscale(2 * HEIGHT / 2, v)));
}

t_color		colorinit(double red, double green, double blue)
{
	t_color	col;

	col.red = red;
	col.green = green;
	col.blue = blue;
	return (col);
}
