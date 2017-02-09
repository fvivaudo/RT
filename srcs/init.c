/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:57:51 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/01/09 13:39:27 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* init objets + lights */

#include <rtv1.h>
#include <stdio.h>
#include <math.h>

void				ft_lstaddobj(t_obj **alst, t_obj *new)
{
	if (alst && *alst && new)
	{
		new->next = *alst;
		*alst = new;
	}
	else if (alst)
		*alst = new;
}

void				init_cyl(t_env *e, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;

	obj = (t_obj*)malloc(sizeof(t_obj));
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return;
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("MATERIAL", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3] && buffer[y + 4] && buffer[y + 5] && buffer[y + 6] && buffer[y + 7] && buffer[y + 8])
			{
				material.diffuse = colorinit(ft_datoi(buffer[y + 1]),
					ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				material.reflection = ft_datoi(buffer[y + 4]);
				material.brillance = ft_datoi(buffer[y + 5]);
				material.bump = ft_datoi(buffer[y + 6]);
				material.refraction = ft_datoi(buffer[y + 7]);
				material.transparency = ft_datoi(buffer[y + 8]);
				y += 9;
			}
			else
				return;
		}
		else if (!ft_strcmp("RADIUS", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->rad = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
				return;
		}
		else if (!ft_strcmp("ORIENTATION", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3])
			{
				obj->dir = vectorinit(ft_datoi(buffer[y + 1]),
				ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				vectornormalize(&obj->dir);
	/*			t_vec rot_axis = vectorproduct(cylinder->dir, vectorinit(1, 0, 0));
				double rot_angle = acos(vectordot(cylinder->dir, vectorinit(1, 0, 0)));
				cylinder->dir.x = rot_axis.x * sin(rot_angle / 2) * e->newstart.x + cylinder->pos.x;
				cylinder->dir.y = rot_axis.y * sin(rot_angle / 2) * e->newstart.y + cylinder->pos.y;
				cylinder->dir.z = rot_axis.z * sin(rot_angle / 2) * e->newstart.z + cylinder->pos.z;
				vectornormalize(&cylinder->dir);*/
				y += 4;
			}
			else
				return;
		}
		else if (!ft_strcmp("HEIGHT", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->height = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
				return;
		}
		else
			++y;
	}
	obj->type = TYPE_CYLINDER;
	obj->material = material;
	obj->pos = obj->pos;
	obj->next = NULL;
	obj->id = e->id;
	ft_lstaddobj(&e->obj, obj);
}

void	init_cone(t_env *e, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;

	obj = (t_obj*)malloc(sizeof(t_obj));
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return;
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("MATERIAL", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3] && buffer[y + 4] && buffer[y + 5] && buffer[y + 6] && buffer[y + 7] && buffer[y + 8])
			{
				material.diffuse = colorinit(ft_datoi(buffer[y + 1]),
					ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				material.reflection = ft_datoi(buffer[y + 4]);
				material.brillance = ft_datoi(buffer[y + 5]);
				material.bump = ft_datoi(buffer[y + 6]);
				material.refraction = ft_datoi(buffer[y + 7]);
				material.transparency = ft_datoi(buffer[y + 8]);
				y += 9;
			}
			else
				return;
		}
		else if (!ft_strcmp("ANGLE", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->rad = ft_datoi(buffer[y + 1]);
				obj->alpha = obj->rad * (M_PI / 180);
				y += 2;
			}
			else
				return;
		}
		else if (!ft_strcmp("ORIENTATION", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3])
			{
				obj->dir = vectorinit(ft_datoi(buffer[y + 1]),
				ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				vectornormalize(&obj->dir);
				y += 4;
			}
			else
				return;
		}
		else
			++y;
	}
	obj->type = TYPE_CONE;
	obj->material = material;
	obj->next = NULL;
	obj->id = e->id;
	ft_lstaddobj(&e->obj, obj);
}

void		init_sphere(t_env *e, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;

	obj = (t_obj*)malloc(sizeof(t_obj));
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return;
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("MATERIAL", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3] && buffer[y + 4] && buffer[y + 5] && buffer[y + 6] && buffer[y + 7] && buffer[y + 8])
			{
				material.diffuse = colorinit(ft_datoi(buffer[y + 1]),
					ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				material.reflection = ft_datoi(buffer[y + 4]);
				material.brillance = ft_datoi(buffer[y + 5]);
				material.bump = ft_datoi(buffer[y + 6]);
				material.refraction = ft_datoi(buffer[y + 7]);
				material.transparency = ft_datoi(buffer[y + 8]);
				y += 9;
			}
			else
				return;
		}
		else if (!ft_strcmp("RADIUS", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->rad = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
				return;
		}
		else
			++y;
	}
	obj->type = TYPE_SPHERE;
	obj->material = material;
	obj->pos = obj->pos;
	obj->next = NULL;
	obj->id = e->id;
	ft_lstaddobj(&e->obj, obj);
}

void		init_plane(t_env *e, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;


	obj = (t_obj*)malloc(sizeof(t_obj));
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
	{
		return;
	}
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("MATERIAL", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3] && buffer[y + 4] && buffer[y + 5] && buffer[y + 6] && buffer[y + 7] && buffer[y + 8])
			{
				material.diffuse = colorinit(ft_datoi(buffer[y + 1]),
					ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				material.reflection = ft_datoi(buffer[y + 4]);
				material.brillance = ft_datoi(buffer[y + 5]);
				material.bump = ft_datoi(buffer[y + 6]);
				material.refraction = ft_datoi(buffer[y + 7]);
				material.transparency = ft_datoi(buffer[y + 8]);
				y += 9;
			}
			else
			{
				return;
			}
		}
		else if (!ft_strcmp("ORIENTATION", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3])
			{
				obj->dir = vectorinit(ft_datoi(buffer[y + 1]),
				ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				vectornormalize(&obj->dir);
				y += 4;
			}
			else
			{
				return;
			}
		}
		else
		{
			++y;
		}
	}
	obj->type = TYPE_PLANE;
	obj->material = material;
	obj->pos = obj->pos;
	obj->next = NULL;
	obj->id = e->id;
	ft_lstaddobj(&e->obj, obj);
}

void		init_quadric(t_env *e, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;
	t_quadric	quad;

	obj = (t_obj*)malloc(sizeof(t_obj));
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
	{
		return;
	}
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("MATERIAL", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3] && buffer[y + 4] && buffer[y + 5] && buffer[y + 6] && buffer[y + 7] && buffer[y + 8])
			{
				material.diffuse = colorinit(ft_datoi(buffer[y + 1]),
					ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				material.reflection = ft_datoi(buffer[y + 4]);
				material.brillance = ft_datoi(buffer[y + 5]);
				material.bump = ft_datoi(buffer[y + 6]);
				material.refraction = ft_datoi(buffer[y + 7]);
				material.transparency = ft_datoi(buffer[y + 8]);
				y += 9;
			}
			else
			{
				return;
			}
		}
		else if (!ft_strcmp("ORIENTATION", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3])
			{
				obj->dir = vectorinit(ft_datoi(buffer[y + 1]),
				ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				vectornormalize(&obj->dir);
				y += 4;
			}
			else
			{
				return;
			}
		}
		else if (!ft_strcmp("PARAM", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3] && buffer[y + 4] && buffer[y + 5] && buffer[y + 6] && buffer[y + 7] && buffer[y + 8] && buffer[y + 9] && buffer[y + 10])
			{
				quad.a = ft_datoi(buffer[y + 1]);
				quad.b = ft_datoi(buffer[y + 2]);
				quad.c = ft_datoi(buffer[y + 3]);
				quad.f = ft_datoi(buffer[y + 4]);
				quad.g = ft_datoi(buffer[y + 5]);
				quad.h = ft_datoi(buffer[y + 6]);
				quad.p = ft_datoi(buffer[y + 7]);
				quad.q = ft_datoi(buffer[y + 8]);
				quad.r = ft_datoi(buffer[y + 9]);
				quad.d = ft_datoi(buffer[y + 10]);
				y += 11;
//printf("quad.a = %g\nquad.b = %g\nquad.c = %g\nquad.d = %g\nquad.e = %g\nquad.f = %g\nquad.g = %g\nquad.h = %g\nquad.i = %g\nquad.j = %g\n", quad.a, quad.b, quad.c, quad.d, quad.e, quad.f, quad.g, quad.h, quad.i, quad.j);
			}
			else
			{
				return;
			}
		}
		else
		{
			++y;
		}
	}

	//we assume the default orientation is toward +y, then we move according to the orientation defined in the configuration file
	double	rot_angle;
	t_vec	rot_axis;
	t_vec	y_axis;

	y_axis = vectorinit(0, 1, 0);
	rot_angle = acos(vectordot(y_axis, obj->dir));
	rot_axis = vectorproduct(y_axis, obj->dir);

	//obj->quad = quadricrotate(quad, vectorproduct(obj->dir, vectorinit(0, 1, 0)), acos(vectordot(vectorinit(0, 1, 0), obj->dir)));
	if (rot_angle)
	{
		obj->quad = quadricrotate(quad, rot_axis, rot_angle, obj->pos);
	}
	else
	{
		obj->quad = quadricrotate(quad, y_axis, 0, obj->pos);
	}
//	obj->quad = quadricrotate(quad, vectorinit(1, 0, 0), 90);
//	obj->quad = quad;
	obj->type = TYPE_QUADRIC;
	obj->material = material;
	//obj->pos = obj->pos;
	obj->next = NULL;
	obj->id = e->id;
	ft_lstaddobj(&e->obj, obj);
}

void	ft_lstaddlight(t_light **alst, t_light *new)
{
	if (alst && *alst && new)
	{
		new->next = *alst;
		*alst = new;
	}
	else if (alst)
		*alst = new;
}

void	init_light(t_env *e, char **buffer)
{
	int			y;
	t_light		*light;

	light = (t_light*)malloc(sizeof(t_light));
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		light->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
	{
		return;
	}
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("INTENSITY", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3])
			{
				light->intensity = colorinit(ft_datoi(buffer[y + 1]),
				ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				y += 4;
			}
			else
			{
				return;
			}
		}
		else
		{
			++y;
		}
	}
	light->next = NULL;
	ft_lstaddlight(&e->lights, light);
}
