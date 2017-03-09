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


bool				setmat(char **buffer, int *y, t_mat *mat)
{
	if (!ft_strcmp("MATERIAL", buffer[*y]))
	{
		if (buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3] && buffer[*y + 4] && buffer[*y + 5] && buffer[*y + 6] && buffer[*y + 7] && buffer[*y + 8])
		{
			mat->diffuse = colorinit(ft_datoi(buffer[*y + 1]),
				ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
			mat->reflection = ft_datoi(buffer[*y + 4]);
			mat->brillance = ft_datoi(buffer[*y + 5]);
			mat->bump = ft_datoi(buffer[*y + 6]);
			mat->refraction = ft_datoi(buffer[*y + 7]);
			mat->transparency = ft_datoi(buffer[*y + 8]);
			*y += 9;
			return (TRUE);
		}
	}
	return (FALSE);
}

bool				setorient(char **buffer, int *y, t_obj *obj)
{
	if (!ft_strcmp("ORIENTATION", buffer[*y]))
	{
		if (buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3])
		{
			obj->dir = vectorinit(ft_datoi(buffer[*y + 1]),
			ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
			vectornormalize(&obj->dir);
			*y += 4;
			return (TRUE);
		}
	}
	return (FALSE);
}

bool				setslice(char **buffer, int *y, t_obj *obj)
{
	t_obj *slice;

	if (!ft_strcmp("SLICE", buffer[*y]))
	{
		if (buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3] && buffer[*y + 4] && buffer[*y + 5] && buffer[*y + 6])
		{
			slice = (t_obj*)malloc(sizeof(t_obj));
			slice->pos = vectorinit(ft_datoi(buffer[*y + 1]),
			ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
			slice->dir = vectorinit(ft_datoi(buffer[*y + 4]),
			ft_datoi(buffer[*y + 5]), ft_datoi(buffer[*y + 6]));
			//printf("slice->dir.x = %g slice->dir.y = %g slice->dir.z = %g\n", slice->dir.x, slice->dir.y, slice->dir.z);
			vectornormalize(&slice->dir);
			lstaddslice(&obj->nextslice, slice);
			*y += 7;
			return (TRUE);
		}
	}
	return (FALSE);
}

bool				setnegative(char **buffer, int *y, t_obj *parent, t_obj **lstobj)
{
//	t_obj *neg;

	if (!ft_strcmp("NEGATIVE", buffer[*y]))
	{
		//printf("neg = %s\n", *(buffer + *y + 1));
		if (!(ft_strcmp(buffer[*y + 1], "SPHERE")))
		{
			*y += init_sphere(lstobj, &(*(buffer + *y + 1)));
			(*lstobj)->normal = normalsphere;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "CONE")))
		{
			*y += init_cone(lstobj, &(*(buffer + *y + 1)));
			(*lstobj)->normal = normalcone;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "CYLINDER")))
		{
			*y += init_cyl(lstobj, &(*(buffer + *y + 1)));
			(*lstobj)->normal = normalcylinder;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "PLANE")))
		{
			*y += init_plane(lstobj, &(*(buffer + *y + 1)));
			(*lstobj)->normal = normalplane;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "QUADRIC")))
		{
			*y += init_quadric(lstobj, &(*(buffer + *y + 1)));
			(*lstobj)->normal = normalquadric;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "OBJECT")))
		{
			*y += init_object(lstobj, &(*(buffer + *y + 1)));
		}
		(*lstobj)->pos = vectoradd(parent->pos, (*lstobj)->pos); // add something for direction too?
	//	printf("parent->pos.x = %g, parent->pos.y = %g, parent->pos.z = %g\n", parent->pos.x, parent->pos.y, parent->pos.z);
	//	printf("(*lstobj)->pos.x = %g, (*lstobj)->pos.y = %g, (*lstobj)->pos.z = %g\n", (*lstobj)->pos.x, (*lstobj)->pos.y, (*lstobj)->pos.z);
	//	(*lstobj)->dir = vectorsub(parent->pos, (*lstobj)->pos);
	}
	return (FALSE);
}

t_obj 				*init_null(void)
{
	t_obj * obj;

	obj = (t_obj*)malloc(sizeof(t_obj));
	obj->id = 0;
	obj->nextitem = NULL;
	obj->nextchild = NULL;
	obj->nextslice = NULL;
	obj->nextneg = NULL;
	obj->rotation = 0;
	return(obj);
}

int				init_cyl(t_obj **lstobj, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;

	obj = init_null();
	y = 4;

	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("ID", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->id = ft_atoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else if (setmat(buffer, &y, &material));
		else if (!ft_strcmp("RADIUS", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->rad = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else if (setorient(buffer, &y, obj));
		else if (setslice(buffer, &y, obj));
		else if (setnegative(buffer, &y, obj, &obj->nextneg));
		else if (!ft_strcmp("HEIGHT", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->height = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else
			++y;
	}
	obj->type = TYPE_CYLINDER;
	obj->material = material;
	obj->pos = obj->pos;
	obj->nextitem = NULL;
	//obj->id = e->id;
	lstaddobj(lstobj, obj);
	return (y);
}

int		init_cone(t_obj **lstobj, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;

	obj = init_null();

	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("ID", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->id = ft_atoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else if (setmat(buffer, &y, &material));
		else if (!ft_strcmp("ANGLE", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->rad = ft_datoi(buffer[y + 1]);
				obj->alpha = obj->rad * (M_PI / 180);
				y += 2;
			}
			else
				return (y);
		}
		else if (setorient(buffer, &y, obj));
		else if (setslice(buffer, &y, obj));
		else if (setnegative(buffer, &y, obj, &obj->nextneg));
		else
			++y;
	}
	obj->type = TYPE_CONE;
	obj->material = material;
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
}

int		init_sphere(t_obj **lstobj, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;

	obj = init_null();
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("ID", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->id = ft_atoi(buffer[y + 1]);
				y += 2;
			}
			else
			{
				return (y);
			}
		}
		else if (setmat(buffer, &y, &material));
		else if (setslice(buffer, &y, obj));
		else if (setnegative(buffer, &y, obj, &obj->nextneg));
		else if (!ft_strcmp("RADIUS", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->rad = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
			{
				return (y);
			}
		}
		else
			++y;
	}
	obj->type = TYPE_SPHERE;
	obj->material = material;
	obj->pos = obj->pos;
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
}

int		init_plane(t_obj **lstobj, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;


	obj = init_null();
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
	{
		return (0);
	}
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("ID", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->id = ft_atoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else if (setmat(buffer, &y, &material));
		else if (setorient(buffer, &y, obj));
		else if (setslice(buffer, &y, obj));
		else if (setnegative(buffer, &y, obj, &obj->nextneg));
		else
		{
			++y;
		}
	}
	obj->type = TYPE_PLANE;
	obj->material = material;
	obj->pos = obj->pos;
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
}

int		init_quadric(t_obj **lstobj, char **buffer)
{
	int			y;
	t_obj		*obj;
	t_mat		material;
	t_quadric	quad;

	obj = init_null();
	
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
	{
		return (0);
	}
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("ID", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->id = ft_atoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else if (setmat(buffer, &y, &material));
		else if (setorient(buffer, &y, obj));
		else if (setslice(buffer, &y, obj));
		else if (setnegative(buffer, &y, obj, &obj->nextneg));
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
				return (y);
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
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
}

//init compose only sets up components and remove them from the scene, making it and invisible object
void		init_compose(t_obj **lstobj, char **buffer)
{
	int 		y;
	t_obj		*obj;

	y = 1;
	obj = init_null();

	while (buffer[y] != NULL)
	{
		//ft_putendl("ok1");
		if (!ft_strcmp("ID", buffer[y]))
		{
			//ft_putendl("typestart1");
			if (buffer[y + 1])
			{
				obj->id = ft_atoi(buffer[y + 1]);
				y += 2;
			}
			else
			{
				return;
			}
		}
		else
		{
			//remove from object list and add to composed object
		//printf("cursor id = %d\n", ft_atoi(buffer[y]));

			lstaddobj(&obj->nextchild, lstremoveoneobj(lstobj, ft_atoi(buffer[y])));
			++y;
		}
//		ft_putendl("ok2");
	}
	lstaddobj(lstobj, obj);
//	ft_putendl("end");
}

t_obj	*copyobj(t_obj obj)
{
	t_obj *copy;

	copy = (t_obj*)malloc(sizeof(t_obj));
	copy->id = 0;
	//copy->id = obj->id;
	//copy->parent = obj->parent;
	copy->type = obj.type;
	copy->material = obj.material;
	copy->pos = obj.pos;
	copy->dir = obj.dir;
	copy->rad = obj.rad;
	copy->height = obj.height;
	copy->alpha = obj.alpha;
	copy->quad = obj.quad;
	copy->negative = obj.negative;
	copy->rotation = obj.rotation;
	copy->nextneg = obj.nextneg;
	copy->nextslice = obj.nextslice;
	return (copy);
}

void		extractobj(t_obj **lstobj, t_obj *obj)
{
	t_obj *tmp;
	t_obj *cursor;

	cursor = obj->nextchild;
	while (cursor)
	{
		//printf("cursor id = %d\n", cursor->id);
		if (cursor->nextchild)
		{
			cursor->rotation = obj->rotation;
			cursor->pos = obj->pos;
			cursor->dir = obj->dir;
			//sub composed objects inherit rotation, position and attributes for now, needs to be changed
			extractobj(lstobj, cursor);
		}
		else
		{
			tmp = copyobj(*cursor);
			t_vec worldfd = vectorinit(0, 0, 1); //forward of world, forward of object is dir

			double rot_angle = acos(vectordot(worldfd,  obj->dir));
			t_vec rot_axis = vectorproduct(worldfd,  obj->dir);
			//get rotation between world and object direction
			vectornormalize(&rot_axis);

			tmp->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir, tmp->pos, obj->rotation);
			tmp->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir, tmp->dir, obj->rotation);
			rot_angle = acos(vectordot(worldfd, obj->dir));
			rot_axis = vectorproduct(worldfd, obj->dir);
			//tmp->dir = vectorrotate(tmp->dir, rot_axis, rot_angle);

		//	printf("obj->dir.x = %g, obj->dir.y = %g, obj->dir.z = %g\n", obj->dir.x, obj->dir.y, obj->dir.z);
			if (rot_axis.x || rot_axis.y || rot_axis.z)
			{
		//		printf("0 tmp->pos.x = %g, tmp->pos.y = %g, tmp->pos.z = %g\n", tmp->pos.x, tmp->pos.y, tmp->pos.z);
				tmp->pos = vectorrotate(tmp->pos, rot_axis, rot_angle);
		//		printf("1 tmp->pos.x = %g, tmp->pos.y = %g, tmp->pos.z = %g\n", tmp->pos.x, tmp->pos.y, tmp->pos.z);
				tmp->pos = vectoradd(tmp->pos, obj->pos);
		//		printf("2 tmp->pos.x = %g, tmp->pos.y = %g, tmp->pos.z = %g\n", tmp->pos.x, tmp->pos.y, tmp->pos.z);
			}
			else
			{
				tmp->pos = vectoradd(tmp->pos, obj->pos);
			}
	//		printf("obj->pos.x = %g, obj->pos.y = %g, obj->pos.z = %g\n", obj->pos.x, obj->pos.y, obj->pos.z);
			lstaddobj(lstobj, tmp);
		}
		cursor = cursor->nextitem;
	}
}

//init object will set up the composed object rotation and position and allow it to be rendered
// by copying its component into the scene
int			init_object(t_obj **lstobj, char **buffer)
{
	int 		y;
	t_obj		*obj;
	t_obj		*cursor;

	obj = NULL;
	y = 2;
	//obj = (t_obj*)malloc(sizeof(t_obj));

	if (buffer[1])
	{
		cursor = *lstobj;
		while (cursor)
		{
			if (cursor->id == ft_atoi(buffer[1]))
			{
				obj = cursor;
			}
			cursor = cursor->nextitem;
		}
	}
	if (buffer == NULL)
	{
		return (1);
	}
	while (buffer[y] != NULL)
	{
		if (!ft_strcmp("POSITION", buffer[y]))
		{
			//ft_putendl("typestart2");
			if (buffer[y + 1] && buffer[y + 2] && buffer[y + 3])
			{
				obj->pos = vectorinit(ft_datoi(buffer[y + 1]), ft_datoi(buffer[y + 2]), ft_datoi(buffer[y + 3]));
				y += 4;
			}
			else
			{
				return (y);
			}
		}
		else if (setorient(buffer, &y, obj));
		else if (setslice(buffer, &y, obj));
		else if (setnegative(buffer, &y, obj, &obj->nextneg));
		else if (!ft_strcmp("ROTATE", buffer[y]))
		{
			if (buffer[y + 1])
			{
				obj->rotation = ft_datoi(buffer[y + 1]) * M_PI / 180.0;
				y += 2;
			}
		}
		else
		{
			++y;
		}
	}
	//ft_putendl("ok2");
	extractobj(lstobj, obj);
	return (y);
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
	lstaddlight(&e->lights, light);
}
