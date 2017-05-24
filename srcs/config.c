/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:57:51 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/17 16:34:23 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* init objets + lights */

#include <rtv1.h>
#include <stdio.h>
#include <math.h>

void readConfig_func(t_env *e, char **buffer_line)
{
	if (!(ft_strcmp(buffer_line[0], "SPHERE")))
		init_sphere(&e->obj, buffer_line, FALSE);
	else if (!(ft_strcmp(buffer_line[0], "CONE")))
		init_cone(&e->obj, buffer_line, FALSE);
	else if (!(ft_strcmp(buffer_line[0], "CYLINDER")))
		init_cyl(&e->obj, buffer_line, FALSE);
	else if (!(ft_strcmp(buffer_line[0], "PLANE")))
		init_plane(&e->obj, buffer_line, FALSE);
	else if (!(ft_strcmp(buffer_line[0], "QUADRIC")))
		init_quadric(&e->obj, buffer_line, FALSE);
	else if (!(ft_strcmp(buffer_line[0], "TORUS")))
		init_torus(&e->obj, buffer_line, FALSE);
	else if (!(ft_strcmp(buffer_line[0], "LIGHT")))
		init_light(e, buffer_line);
	else if (!(ft_strcmp(buffer_line[0], "CAMERA")))
		init_cam(e, buffer_line);
	else if (!(ft_strcmp(buffer_line[0], "COMPOSE")))
		init_compose(&e->obj, buffer_line);
	else if (!(ft_strcmp(buffer_line[0], "OBJECT")))
		init_object(&e->obj, buffer_line, FALSE);
	else if (!(ft_strcmp(buffer_line[0], "EFFECT")))
		init_effect(e, buffer_line);
}

t_env			*readConfig(int fd)
{
	char			*buffer_gnl;
	char			**buffer_line;
	t_env			*e;

	e = (t_env*)malloc(sizeof(t_env));
	reset(e, 0, 0);
	e->obj = NULL;
	e->lights = NULL;
	e->id = 0;
	while (get_next_line(fd, &buffer_gnl) == 1)
	{
		buffer_line = ft_strsplitspace(buffer_gnl);
		if (buffer_line[0])
			readConfig_func(e, buffer_line);
		free(buffer_gnl);
		ft_doubletabfree(&buffer_line);
		++e->id;
	}
	close(fd);
	return (e);
}

int 			init_effect(t_env *e, char **buffer)
{
	if (buffer[1] && buffer[2] && buffer[3] && buffer[4])
	{
		e->effect = ft_datoi(buffer[1]);
		e->ered = ft_datoi(buffer[2]);
		e->egreen = ft_datoi(buffer[3]);
		e->eblue = ft_datoi(buffer[4]);
	}
	return (0);
}

typedef struct 	s_init_cyl
{
	int			y;
	t_obj		*obj;
	t_mat		material;
	t_quadric	quad;
}				t_init_cyl;

int 			init_cyl_func(t_init_cyl *u, char **buffer)
{
	u->obj = init_null();
	u->y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u->obj->pos = vectorinit(ft_datoi(buffer[1]),
			ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	return (-1234567);
}

int 			init_cyl_func2(t_init_cyl *u, char **buffer)
{
	if (buffer[u->y + 1])
	{
		u->obj->id = ft_atoi(buffer[u->y + 1]);
		u->y += 2;
	}
	else
		return (u->y);
	return (-1234567);
}

int 			init_cyl_func3(t_init_cyl *u, char **buffer)
{
	if (buffer[u->y + 1])
	{
		u->obj->rad = ft_datoi(buffer[u->y + 1]);
		u->y += 2;
	}
	else
		return (u->y);
	return (-1234567);
}

int 			init_cyl_func4(t_init_cyl *u, char **buffer)
{
	if (buffer[u->y + 1])
	{
		u->obj->height = ft_datoi(buffer[u->y + 1]);
		u->y += 2;
	}
	else
		return (u->y);
	return (-1234567);
}


int 			init_cyl_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
				bool neg)
{
	int res;

	if (!ft_strcmp("ID", buffer[u->y]) &&
		(res = init_cyl_func2(u, buffer)) != -1234567 )
		return (res);
	else if (setmat(buffer, &u->y, &u->material));
	else if (!ft_strcmp("RADIUS", buffer[u->y]))
	{
		if ((res = init_cyl_func3(u, buffer)) != -1234567)
			return (res);
	}
	else if (setorient(buffer, &u->y, u->obj));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y,
		u->obj, &u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else if (!ft_strcmp("HEIGHT", buffer[u->y]))
	{
		if ((res = init_cyl_func4(u, buffer)) != -1234567)
			return (res);
	}
	else
		++u->y;
	return (-1234567);
}


int				init_cyl(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	if (init_cyl_func(&u, buffer) != -1234567)
		return (1);
	while (buffer[u.y] != NULL)
		if ((res = init_cyl_func1(&u, buffer, lstobj, neg)) != -1234567)
			return (res);
	u.obj->type = TYPE_CYLINDER;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}

int 			init_cone_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
				bool neg)
{
	int res;

	if (!ft_strcmp("ID", buffer[u->y]) &&
		(res = init_cyl_func2(u, buffer)) != -1234567 )
		return (res);
	else if (setmat(buffer, &u->y, &u->material));
	else if (!ft_strcmp("ANGLE", buffer[u->y]))
	{
		if (buffer[u->y + 1])
		{
			u->obj->rad = ft_datoi(buffer[u->y + 1]);
			u->obj->alpha = u->obj->rad * (M_PI / 180);
			u->y += 2;
		}
		else
			return (u->y);
	}
	else if (setorient(buffer, &u->y, u->obj));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y,
		u->obj, &u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else
		++u->y;
	return (-1234567);
}




int		init_cone(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	u.obj = init_null();
	u.y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u.obj->pos = vectorinit(ft_datoi(buffer[1]),
		ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[u.y] != NULL)
		if ((res = init_cone_func1(&u, buffer, lstobj, neg)) != -1234567)
			return (res);
	u.obj->type = TYPE_CONE;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}

int 			init_sphere_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
	bool neg)
{
	int res;

	if (!ft_strcmp("ID", buffer[u->y]) &&
		(res = init_cyl_func2(u, buffer)) != -1234567)
		return (res);
	else if (setmat(buffer, &u->y, &u->material));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y, u->obj,
		&u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else if (!ft_strcmp("RADIUS", buffer[u->y]))
	{
		if (buffer[u->y + 1])
		{
			u->obj->rad = ft_datoi(buffer[u->y + 1]);
			u->y += 2;
		}
		else
			return (u->y);
	}
	else
		++u->y;
	return (-1234567);
}

int 			init_plane_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
	bool neg)
{
	if (!ft_strcmp("ID", buffer[u->y]))
	{
		if (buffer[u->y + 1])
		{
			u->obj->id = ft_atoi(buffer[u->y + 1]);
			u->y += 2;
		}
		else
			return (u->y);
	}
	else if (setmat(buffer, &u->y, &u->material));
	else if (setorient(buffer, &u->y, u->obj));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y, u->obj,
		 &u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else
		++u->y;
	return (-1234567);
}

int		init_sphere(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	u.obj = init_null();
	u.y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u.obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]),
		ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[u.y] != NULL)
	if ((res = init_sphere_func1(&u, buffer, lstobj, neg)) != -1234567)
		return (res);
	u.obj->type = TYPE_SPHERE;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}

int		init_plane(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	u.obj = init_null();
	u.y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u.obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]),
		ft_datoi(buffer[3]));
	else
		return (0);
	while (buffer[u.y] != NULL)
	if ((res = init_plane_func1(&u, buffer, lstobj, neg)) != -1234567)
		return (res);
	u.obj->type = TYPE_PLANE;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}

void 			init_quadric_func2(t_init_cyl *u, char **buffer)
{
	u->quad.a = ft_datoi(buffer[u->y + 1]);
	u->quad.b = ft_datoi(buffer[u->y + 2]);
	u->quad.c = ft_datoi(buffer[u->y + 3]);
	u->quad.f = ft_datoi(buffer[u->y + 4]);
	u->quad.g = ft_datoi(buffer[u->y + 5]);
	u->quad.h = ft_datoi(buffer[u->y + 6]);
	u->quad.p = ft_datoi(buffer[u->y + 7]);
	u->quad.q = ft_datoi(buffer[u->y + 8]);
	u->quad.r = ft_datoi(buffer[u->y + 9]);
	u->quad.d = ft_datoi(buffer[u->y + 10]);
	u->y += 11;
}

int 			init_quadric_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
	bool neg)
{
	int res;

	if (!ft_strcmp("ID", buffer[u->y]) &&
		(res = init_cyl_func2(u, buffer)) != -1234567)
		return (res);
	else if (setmat(buffer, &u->y, &u->material));
	else if (setorient(buffer, &u->y, u->obj));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y, u->obj,
		&u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else if (!ft_strcmp("PARAM", buffer[u->y]))
	{
		if (buffer[u->y + 1] && buffer[u->y + 2] && buffer[u->y + 3]
			&& buffer[u->y + 4] && buffer[u->y + 5] && buffer[u->y + 6]
			&& buffer[u->y + 7] && buffer[u->y + 8] && buffer[u->y + 9]
			&& buffer[u->y + 10])
			init_quadric_func2(u, buffer);
		else
			return (u->y);
	}
	else
		++u->y;
	return (-1234567);
}
int		init_quadric(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	u.obj = init_null();
	u.y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u.obj->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]),
		ft_datoi(buffer[3]));
	else
		return (0);
	while (buffer[u.y] != NULL)
	if ((res = init_quadric_func1(&u, buffer, lstobj, neg)) != -1234567)
		return (res);
	double	rot_angle;
	t_vec	rot_axis;
	t_vec	y_axis;
	y_axis = vectorinit(0, 1, 0);
	rot_angle = acos(vectordot(y_axis, u.obj->dir));
	rot_axis = vectorproduct(y_axis, u.obj->dir);
	if (rot_angle)
		u.obj->quad = quadricrotate(u.quad, rot_axis, rot_angle, u.obj->pos);
	else
		u.obj->quad = quadricrotate(u.quad, y_axis, 0, u.obj->pos);
	u.obj->type = TYPE_QUADRIC;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}

int 			init_torus_func1(t_init_cyl *u, char **buffer, t_obj **lstobj,
	bool neg)
{
	int res;

	if (!ft_strcmp("ID", buffer[u->y]) &&
		(res = init_cyl_func2(u, buffer)) != -1234567)
		return (res);
	else if (setmat(buffer, &u->y, &u->material));
	else if (setorient(buffer, &u->y, u->obj));
	else if (setslice(buffer, &u->y, u->obj));
	else if (neg == FALSE && setnegative(buffer, &u->y,
		u->obj, &u->obj->nextneg));
	else if (neg == TRUE && setnegative(buffer, &u->y, NULL, lstobj));
	else if (!ft_strcmp("RADIUS", buffer[u->y]))
	{
		if (buffer[u->y + 1] && buffer[u->y + 2])
		{
			u->obj->rad = ft_datoi(buffer[u->y + 1]);
			u->obj->rad2 = ft_datoi(buffer[u->y + 2]);
			u->y += 3;
		}
		else
			return (u->y);
	}
	else
		++u->y;
	return (-1234567);
}

int					init_torus(t_obj **lstobj, char **buffer, bool neg)
{
	t_init_cyl u;
	int res;

	u.obj = init_null();
	u.y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		u.obj->pos = vectorinit(ft_datoi(buffer[1]),
		ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[u.y] != NULL)
	if ((res = init_torus_func1(&u, buffer, lstobj, neg)) != -1234567)
		return (res);
	u.obj->type = TYPE_TORUS;
	u.obj->material = u.material;
	u.obj->nextitem = NULL;
	lstaddobj(lstobj, u.obj);
	return (u.y);
}


t_quadric initquad(double param[10])
{
	t_quadric quad;

	quad.a = param[0];
	quad.b = param[1];
	quad.c = param[2];
	quad.f = param[3];
	quad.g = param[4];
	quad.h = param[5];
	quad.p = param[6];
	quad.q = param[7];
	quad.r = param[8];
	quad.d = param[9];
	return (quad);
}

typedef struct s_ncomp
{
	int 		y;
	t_obj		*obj;
	t_obj		*tmp;
}				t_ncomp;

void		init_compose(t_obj **lstobj, char **buffer)
{
	t_ncomp u;

	u.y = 1;
	u.obj = init_null();
	while (buffer[u.y] != NULL)
	{
		if (!ft_strcmp("ID", buffer[u.y]))
		{
			if (buffer[u.y + 1])
			{
				u.obj->id = ft_atoi(buffer[u.y + 1]);
				u.y += 2;
			}
			else
				return;
		}
		else
		{
			while (*lstobj && (u.tmp = lstremoveoneobj(lstobj,
				ft_atoi(buffer[u.y]))))
				lstaddobj(&u.obj->nextchild, u.tmp);
			++u.y;
		}
	}
	lstaddobj(lstobj, u.obj);
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
	obj->isneg = FALSE;
	obj->type_obj = TYPE_OBJECT;
	return(obj);
}

void 			setmat_func(t_mat *mat, int *y, char **buffer)
{
	mat->diffuse = colorinit(ft_datoi(buffer[*y + 1]),
		ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
	mat->reflection = ft_datoi(buffer[*y + 4]);
	mat->brillance = ft_datoi(buffer[*y + 5]);
	mat->bump = ft_datoi(buffer[*y + 6]);
	mat->refraction = ft_datoi(buffer[*y + 7]);
	mat->transparency = ft_datoi(buffer[*y + 8]);
	mat->type = ft_datoi(buffer[*y + 9]);
}

bool			 	setmat(char **buffer, int *y, t_mat *mat)
{
	if (!ft_strcmp("MATERIAL", buffer[*y]))
	{
		if (buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3] &&
			buffer[*y + 4] && buffer[*y + 5] && buffer[*y + 6] &&
			buffer[*y + 7] && buffer[*y + 8] && buffer[*y + 9])
		{
			setmat_func(mat, y, buffer);
			*y += 10;
		}
		if (mat->type > 0 && buffer[*y] && buffer[*y + 1] && buffer[*y + 2]
			&& buffer[*y + 3])
		{
			mat->procedural_scale = ft_datoi(buffer[*y]);
			mat->diffuse2 = colorinit(ft_datoi(buffer[*y + 1]),
				ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
			*y += 4;
		}
		return (TRUE);
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
		if (buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3] &&
			buffer[*y + 4] && buffer[*y + 5] && buffer[*y + 6])
		{
			slice = (t_obj*)malloc(sizeof(t_obj));
			slice = init_null();
			slice->pos = vectorinit(ft_datoi(buffer[*y + 1]),
			ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
			slice->dir = vectorinit(ft_datoi(buffer[*y + 4]),
			ft_datoi(buffer[*y + 5]), ft_datoi(buffer[*y + 6]));
			vectornormalize(&slice->dir);
			slice->type_obj = TYPE_SLICE;
			lstaddslice(&obj->nextslice, slice);
			*y += 7;
			return (TRUE);
		}
	}
	return (FALSE);
}

bool				setnegative(char **buffer, int *y, t_obj *parent,
	 t_obj **lstobj)
{
	static t_obj* parentsave = NULL;

	if (parent)
		parentsave = parent;
	if (!ft_strcmp("NEGATIVE", buffer[*y]))
	{
		if (!(ft_strcmp(buffer[*y + 1], "SPHERE")))
			*y += init_sphere(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "CONE")))
			*y += init_cone(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "CYLINDER")))
			*y += init_cyl(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "PLANE")))
			*y += init_plane(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "QUADRIC")))
			*y += init_quadric(lstobj, &(*(buffer + *y + 1)), TRUE);
		else if (!(ft_strcmp(buffer[*y + 1], "OBJECT")))
			*y += init_object(lstobj, &(*(buffer + *y + 1)), TRUE);
		(*lstobj)->pos = vectoradd(parentsave->pos, (*lstobj)->pos);
		(*lstobj)->type_obj = TYPE_NEGATIVE;
		(*lstobj)->isneg = TRUE;
		return (TRUE);
	}
	return (FALSE);
}

typedef struct s_incam
{
	t_vec	up;
	t_vec	u;
	t_vec	v;

	t_vec	tmp_up;
	t_vec	tmp_vdir;
}				t_incam;

void 		init_cam_func(t_env *e, t_incam *o)
{
	e->cam.lookat = vectorscale(SCREEN_EYE_DIST, e->cam.vdir);
	o->tmp_vdir = vectorinit(0, 0, 1);
	o->tmp_up = vectorinit(0, -1, 0);
	o->up = vectorinit(0, -1, 0);
	o->u = vectorproduct(e->cam.vdir, o->up);
	o->v = vectorproduct(o->u, e->cam.vdir);
	o->u = vectornormalize(&o->u);
	o->v = vectornormalize(&o->v);
	e->cam.viewplanebottomleftpoint = vectorsub(
	vectorsub(e->cam.lookat, vectorscale(HEIGHT / 2, o->v)),
		vectorscale(WIDTH / 2, o->u));
	e->cam.viewplanebottomleftpoint =
		vectoradd(e->cam.viewplanebottomleftpoint, e->cam.eyepoint);
	e->cam.viewplanebottomleftpoint =
		vectoradd(e->cam.viewplanebottomleftpoint, e->cam.lookat);
	e->cam.xincvector = vectorscale(1 / (double)WIDTH,
	(vectorscale(2 * WIDTH / 2, o->u)));
	e->cam.yincvector = vectorscale(1 / (double)HEIGHT,
	(vectorscale(2 * HEIGHT / 2, o->v)));
}

void		init_cam(t_env *e, char **buffer)
{
	t_incam o;

	if (buffer[1] && buffer[2] && buffer[3])
		e->cam.eyepoint = vectorinit(ft_datoi(buffer[1]),
		ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return;
	if (!ft_strcmp("LOOKAT", buffer[4]) && buffer[5] && buffer[6] && buffer[7])
	{
		e->cam.lookat = vectorinit(ft_datoi(buffer[5]), ft_datoi(buffer[6]),
			ft_datoi(buffer[7]));
		e->cam.vdir = vectorsub(e->cam.lookat, e->cam.eyepoint);
		vectornormalize(&e->cam.vdir);
	}
	else if (!ft_strcmp("ORIENTATION", buffer[4]) && buffer[5] && buffer[6]
		&& buffer[7])
	{
		e->cam.vdir = vectorinit(ft_datoi(buffer[5]), ft_datoi(buffer[6]),
			ft_datoi(buffer[7]));
		vectornormalize(&e->cam.vdir);
	}
	else
		return;
	init_cam_func(e, &o);
}

typedef struct s_niniobj
{
	int 		y;
	t_obj		*obj;
	t_obj		*cursor;
	int 		id;
}				t_niniobj;

int 			init_obj_func2(t_niniobj *u, char **buffer)
{
	if (buffer[u->y + 1])
	{
		u->obj->id = ft_atoi(buffer[u->y + 1]);
		u->y += 2;
	}
	else
		return (u->y);
	return (-1234567);
}
int 			init_obj_func3(t_niniobj *o, char **buffer)
{
	if (buffer[o->y + 1])
	{
		o->obj->rotation = ft_datoi(buffer[o->y + 1]) * M_PI / 180.0;
		o->y += 2;
	}
	return (-1234567);
}

int 			init_object_func1(t_niniobj *o, char **buffer, t_obj **lstobj,
	bool neg)
{
	int (res);

	if (!ft_strcmp("ID", buffer[o->y]) &&
		(res = init_obj_func2(o, buffer)) != -1234567)
		return (res);
	else if (!ft_strcmp("POSITION", buffer[o->y]))
	{
		if (buffer[o->y + 1] && buffer[o->y + 2] && buffer[o->y + 3])
		{
			o->obj->pos = vectorinit(ft_datoi(buffer[o->y + 1]),
			ft_datoi(buffer[o->y + 2]), ft_datoi(buffer[o->y + 3]));
			o->y += 4;
		}
		else
			return (o->y);
	}
	else if (setorient(buffer, &o->y, o->obj));
	else if (setslice(buffer, &o->y, o->obj));
	else if (neg == FALSE && setnegative(buffer, &o->y, o->obj, OHAO));
	else if (neg == TRUE && setnegative(buffer, &o->y, NULL, lstobj));
	else if (!ft_strcmp("ROTATE", buffer[o->y]))
		init_obj_func3(o, buffer);
	else
		++o->y;
	return (-1234567);
}

int			init_object(t_obj **lstobj, char **buffer, bool neg)
{
	t_niniobj o;
	int res;

	o.obj = NULL;
	o.y = 2;
	o.id = 0;
	if (buffer[1])
	{
		o.cursor = *lstobj;
		while (o.cursor)
		{
			if (o.cursor->id == ft_atoi(buffer[1]))
				o.obj = o.cursor;
			o.cursor = o.cursor->nextitem;
		}
	}
	if (buffer == NULL)
		return (1);
	while (buffer[o.y] != NULL)
	if ((res = init_object_func1(&o, buffer, lstobj, neg)) != -1234567)
		return (res);
	extractobj(lstobj, o.obj, o.id);
	return (o.y);
}

void 	init_light_func(char **buffer, int y, t_light *light)
{
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
				return;
		}
		else
			++y;
	}
}

void	init_light(t_env *e, char **buffer)
{
	int			y;
	t_light		*light;

	light = (t_light*)malloc(sizeof(t_light));
	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		light->pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]),
			ft_datoi(buffer[3]));
	else
		return;
	init_light_func(buffer, y, light);
	light->next = NULL;
	lstaddlight(&e->lights, light);
}



t_obj	*copyobj(t_obj *obj)
{
	t_obj *copy;

	copy = (t_obj*)malloc(sizeof(t_obj));
	ft_memcpy(copy, obj, sizeof(t_obj));
	if (obj->nextneg)
		copy->nextneg = copyobj(obj->nextneg);
	else
		copy->nextneg = NULL;
	if (obj->nextslice)
		copy->nextslice = copyobj(obj->nextslice);
	else
		copy->nextslice = NULL;
	if (obj->nextitem)
		copy->nextitem = copyobj(obj->nextitem);
	else
		copy->nextitem = NULL;
	return (copy);
}

void 		rotateinnercomponents(t_obj *obj, t_obj *child)
{
	t_obj *cursor;

	cursor = child->nextneg;
	while (cursor)
	{
		cursor->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir,
			vectoradd(cursor->dir, cursor->pos), obj->rotation);
		cursor->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir,
			vectoradd(obj->pos, cursor->pos), obj->rotation);
		cursor->dir = vectorsub(cursor->dir, cursor->pos);
		cursor = cursor->nextitem;
	}
	cursor = child->nextslice;
	while (cursor)
	{
		cursor->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir,
			vectoradd(cursor->dir, cursor->pos), obj->rotation);
		cursor->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir,
			vectoradd(obj->pos, cursor->pos), obj->rotation);
		cursor->dir = vectorsub(cursor->dir, cursor->pos);
		cursor->rotation = 0;
		cursor = cursor->nextslice;
	}
}

typedef struct s_extobj
{
	t_obj *tmp;
	t_obj *cursor;
	double rot_angle;
	t_vec worldfd;
	t_vec rot_axis;
}				t_extobj;

void 		extract_obj_func(t_extobj *o, int id, t_obj *obj)
{
	o->tmp = copyobj(o->cursor);
	o->tmp->id = id;
	o->worldfd = vectorinit(0, 0, -1);
	vectornormalize(&o->rot_axis);
	o->tmp->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir,
		vectoradd(o->tmp->dir, o->tmp->pos), obj->rotation);
	o->tmp->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir,
		vectoradd(obj->pos, o->tmp->pos), obj->rotation);
	o->tmp->dir = vectorsub(o->tmp->dir, o->tmp->pos);
	if (obj->rotation && (o->tmp->nextneg || o->tmp->nextslice))
		rotateinnercomponents(obj, o->tmp);
	vectornormalize(&o->tmp->dir);
	o->rot_angle = acos(vectordot(o->worldfd, obj->dir));
	o->rot_axis = vectorproduct(o->worldfd, obj->dir);
	if (o->rot_axis.x || o->rot_axis.y || o->rot_axis.z)
	{
		o->tmp->pos = vectorrotate(o->tmp->pos, o->rot_axis, o->rot_angle);
		o->tmp->pos = vectoradd(o->tmp->pos, obj->pos);
	}
	else
		o->tmp->pos = vectoradd(o->tmp->pos, obj->pos);
}

void		extractobj(t_obj **lstobj, t_obj *obj, int id)
{
	t_extobj o;

	o.cursor = obj->nextchild;
	while (o.cursor)
	{
		if (o.cursor->nextchild)
		{
			if (o.cursor->type == 0)
				o.cursor->pos = vectoradd(o.cursor->pos, obj->pos);
			extractobj(lstobj, o.cursor, id);
		}
		else if (o.cursor->type > 0)
		{
			extract_obj_func(&o, id, obj);
			lstaddobj(lstobj, o.tmp);
		}
		o.cursor = o.cursor->nextitem;
	}
}
