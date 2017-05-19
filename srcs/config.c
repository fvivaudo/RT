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

void env_effect_init(t_effects *e)
{
	e->rgb[0] = 0;
	e->rgb[1] = 0;
	e->rgb[2] = 0;
	e->y = 0;
	e->x = 0;
	e->i = 0;
	e->j = 0;
	e->grayscale = 0;
	e->sepia = 0;
	e->sobel = 0;
	e->blur = 0;
	e->motion_blur = 0;
	e->emboss = 0;
	e->sharpen1 = 0;
	e->sharpen2 = 0;
	e->toon = 0;
	e->anim = 1;
}

// pour chaque line
// 	add to tlist
// pour chaque t_list
//
// 	while tlist->next == ANIM
// 		create interpolation list entre dernier t_list->content et anim
// 		add interpolation to last tlist->content
// 		delete tlist anim

int	ft_isnumber(char *s)
{
	if (*s == '-')
		s++;
	while (*s && ft_isdigit(*s))
	{
		s++;
	}
	if (*s == '.')
			{
				s++;
				while (*s && ft_isdigit(*s))
										s++;

		}
		if (*s)
			return (0);
		else
			return (1);
}

void putlst(t_list *l)
{
	char **content;
	while (l)
	{
		content = l->content;
		while (*content)
			printf("%s ", *content++);
		l = l->next;
		printf(". ");

	}
}


void putfile(t_list *l)
{
	t_list *content;
	while (l)
	{
		content = l->content;
		putlst(content);
		l = l->next;
		printf(" \n");
	}
}

t_list		*get_file(fd)
{
	char			*buffer_gnl;
	char			**buffer_line;
	t_list		*file;
	t_list		*tmp;
	t_list		*line;

	//file = ft_lstnew(1, 0);
	file = NULL;
	while (get_next_line(fd, &buffer_gnl) == 1)
	{
		buffer_line = ft_strsplitspace(buffer_gnl);
		line = ft_lstnew(0, 0);
		line->content = buffer_line;
		line->content_size = sizeof(char **);
//		if (file == NULL)
			//	file = ft_lstnew(line, sizeof(t_list *));
		//else
		tmp = ft_lstnew(0, 0);
		tmp->content = line;
		tmp->content_size = sizeof(t_list *);
		ft_lstaddb(&file, tmp);
		//putlst(line);
		//printf("\n" );

		free(buffer_gnl);
	}
	printf("putfile\n" );
	putfile(file);
	printf("end\n" );
	return(file);
}

double  interpolation_linear(double n1, double n2, double n)
{
  return (n1 + n * (n2 - n1));
}

// get buff1 size
// get nb_frames
// 	creer nb_frames t_list contenant char** (size = buff1_size)
// pour chaque t_list
// 	pacourir tout les buff et inserer valeur

int	ft_pstrlen(char **s)
{
	char **ret;

	ret = s;
	while(*s)
		s++;
	return (s - ret);
}

void interpol_line(t_list* l1, t_list *l2)
{
	int frames;
	char **buff2;
	char **buff1;
	t_list *tmp;
	int i;
	double n;
	int buff_size;

	i = -1;
	tmp = l1;
	frames = 0;
	buff1 = ((char **)l1->content);
	buff2 = ((char **)l2->content);
	buff_size = ft_pstrlen(buff1) - 1;
	frames = ft_atoi(buff2[2]);
	printf("%d\n", frames);
	while (++i < frames)
	{
		tmp->next = ft_lstnew(0,0);
		tmp = tmp->next;
		tmp->content = malloc(buff_size * sizeof(char *));
	}
	tmp = l1->next;
	i = 0;
	while (++i < frames)
	{
		n = (double)i / frames + 0.;
		int j = -1;
		while (++j < buff_size)
		{
			if (ft_isnumber(buff1[j]))
			{
				char s[50];
 				sprintf(s,"%g", interpolation_linear(ft_datoi(buff1[j]),ft_datoi(buff2[3 + j]), n));
				((char **)tmp->content)[j] = ft_strdup(s);
			//	printf("buff1 = %g , buff 2 = %g, n %g", ft_datoi(buff1[j]),ft_datoi(buff2[3 + j]), n);

			}
			else
				((char **)tmp->content)[j] = ft_strdup(buff1[j]);
		}
		tmp = tmp->next;
	}
	tmp->next = l2;
}

void 		set_anim(t_list *file)
{
	t_list *tmp;

	tmp = file;
	while (file->next)
	{
		if (((char **)((t_list *)file->next->content)->content)[0] && !ft_strcmp(((char **)((t_list *)file->next->content)->content)[0], "ANIM"))
		{
				printf("interpol\n");
				interpol_line(file->content, file->next->content);
				//delete file->next
				file->next = file->next->next;
		}
		else
			file = file->next;
	}
	/* code */
}

t_env			*readConfig(int fd)
{
	char			*buffer_gnl;
	char			**buffer_line;
	t_env			*e;
	t_list		*file;
	e = (t_env*)malloc(sizeof(t_env));
	reset(e, 0, 0);
	e->obj = NULL;
	e->lights = NULL;
	e->id = 0;
	env_effect_init(&e->effect);

	file = get_file(fd);
	set_anim(file);
	putfile(file);
	return NULL;
	//e->effect = (t_effects)malloc(sizeof(t_effects));
	while (get_next_line(fd, &buffer_gnl) == 1)
	{
//		ft_putendl(buffer_gnl);
		buffer_line = ft_strsplitspace(buffer_gnl);
		if (buffer_line[0])
		{
			if (!(ft_strcmp(buffer_line[0], "SPHERE")))
			{
				init_sphere(&e->obj, buffer_line, FALSE);
			}
			else if (!(ft_strcmp(buffer_line[0], "CONE")))
			{
				init_cone(&e->obj, buffer_line, FALSE);
			}
			else if (!(ft_strcmp(buffer_line[0], "CYLINDER")))
			{
				init_cyl(&e->obj, buffer_line, FALSE);
			}
			else if (!(ft_strcmp(buffer_line[0], "PLANE")))
			{
				init_plane(&e->obj, buffer_line, FALSE);
			}
			else if (!(ft_strcmp(buffer_line[0], "QUADRIC")))
			{
				init_quadric(&e->obj, buffer_line, FALSE);
			}
			else if (!(ft_strcmp(buffer_line[0], "TORUS")))
			{
				init_torus(&e->obj, buffer_line, FALSE);
			}
			else if (!(ft_strcmp(buffer_line[0], "LIGHT")))
			{
				init_light(e, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "CAMERA")))
			{
				init_cam(e, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "COMPOSE")))
			{
				init_compose(&e->obj, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "OBJECT")))
			{
				init_object(&e->obj, buffer_line, FALSE);
			}
			else if (!(ft_strcmp(buffer_line[0], "EFFECT")))
			{
				init_effect(e, buffer_line);
			//	printf("\neffect %g, red %g , green %g ,  blue %g\n", e->col.effect, e->col.ered, e->col.egreen, e->col.eblue);
			}
		}
		free(buffer_gnl);
		ft_doubletabfree(&buffer_line);
		++e->id;
	}
	close(fd);
	return (e);
}

int 			init_effect(t_env *e, char **buffer)
{
	static double order = 1;
	// if (!ft_strcmp("COLOR", buffer[1]) && buffer[2] && buffer[3] && buffer[4])
	// {
	// 	//e->effect = 1;
	// 	e->ered = ft_datoi(buffer[2]);
	// 	e->egreen = ft_datoi(buffer[3]);
	// 	e->eblue = ft_datoi(buffer[4]);
	// }
	if (!ft_strcmp("SOBEL", buffer[1]))
		e->effect.sobel = order;
	else if (!ft_strcmp("SEPIA", buffer[1]))
		e->effect.sepia = order;
	else if (!ft_strcmp("GRAYSCALE", buffer[1]))
		e->effect.grayscale = order;
	else if (!ft_strcmp("BLUR", buffer[1]))
		e->effect.blur = order;
		else if (!ft_strcmp("MOTION_BLUR", buffer[1]))
			e->effect.motion_blur = order;
	else if (!ft_strcmp("SHARPEN1", buffer[1]))
		e->effect.sharpen1 = order;
		else if (!ft_strcmp("SHARPEN2", buffer[1]))
			e->effect.sharpen2 = order;
		else if (!ft_strcmp("EMBOSS", buffer[1]))
			e->effect.emboss = order;
		else if (!ft_strcmp("TOON", buffer[1]))
			e->effect.toon = order;
		else if (!ft_strcmp("DEPTH", buffer[1]))
		{

		}
		else if (!ft_strcmp("STEREO", buffer[1]))
		{
				e->effect.stereo = 1;
				order--;
		}
		else if (!ft_strcmp("ANIM", buffer[1]))
		{
			printf("anim\n");
				e->effect.anim = ft_atoi(buffer[2]);
				order--;
		}
	order++;
	return (0);
}

int				init_cyl(t_obj **lstobj, char **buffer, bool neg)
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
		else if (neg == FALSE && setnegative(buffer, &y, obj, &obj->nextneg));
		else if (neg == TRUE && setnegative(buffer, &y, NULL, lstobj));
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
	obj->nextitem = NULL;
	//obj->id = e->id;
	lstaddobj(lstobj, obj);
	return (y);
}

int		init_cone(t_obj **lstobj, char **buffer, bool neg)
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
		else if (neg == FALSE && setnegative(buffer, &y, obj, &obj->nextneg));
		else if (neg == TRUE && setnegative(buffer, &y, NULL, lstobj));
		else
			++y;
	}
	obj->type = TYPE_CONE;
	obj->material = material;
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
}

int		init_sphere(t_obj **lstobj, char **buffer, bool neg)
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
		else if (neg == FALSE && setnegative(buffer, &y, obj, &obj->nextneg));
		else if (neg == TRUE && setnegative(buffer, &y, NULL, lstobj));
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
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
}

int		init_plane(t_obj **lstobj, char **buffer, bool neg)
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
		else if (neg == FALSE && setnegative(buffer, &y, obj, &obj->nextneg));
		else if (neg == TRUE && setnegative(buffer, &y, NULL, lstobj));
		else
		{
			++y;
		}
	}
	obj->type = TYPE_PLANE;
	obj->material = material;
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
}

int		init_quadric(t_obj **lstobj, char **buffer, bool neg)
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
		else if (neg == FALSE && setnegative(buffer, &y, obj, &obj->nextneg));
		else if (neg == TRUE && setnegative(buffer, &y, NULL, lstobj));
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
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
}

int					init_torus(t_obj **lstobj, char **buffer, bool neg)
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
		else if (setorient(buffer, &y, obj));
		else if (setslice(buffer, &y, obj));
		else if (neg == FALSE && setnegative(buffer, &y, obj, &obj->nextneg));
		else if (neg == TRUE && setnegative(buffer, &y, NULL, lstobj));
		else if (!ft_strcmp("RADIUS", buffer[y]))
		{
			if (buffer[y + 1] && buffer[y + 2])
			{
				obj->rad = ft_datoi(buffer[y + 1]);
				obj->rad2 = ft_datoi(buffer[y + 2]);
				y += 3;
			}
			else
			{
				return (y);
			}
		}
		else
			++y;
	}
	obj->type = TYPE_TORUS;
	obj->material = material;
	obj->nextitem = NULL;
	lstaddobj(lstobj, obj);
	return (y);
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

//should grab one or several object with same id in the scene and add em all to a list of children
//init compose only sets up components and remove them from the scene, making it and invisible object
void		init_compose(t_obj **lstobj, char **buffer)
{
	int 		y;
	t_obj		*obj;
	t_obj		*tmp;

	y = 1;
	//create new parent
	obj = init_null();

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
				return;
			}
		}
		else
		{
			//remove from object list and add to composed object
			//printf("ID = %d\n", ft_atoi(buffer[y]));

			while (*lstobj && (tmp = lstremoveoneobj(lstobj, ft_atoi(buffer[y]))))
			{
				lstaddobj(&obj->nextchild, tmp);
			}
			/*t_obj *cursor = (obj->nextchild);
			printf("reset\n");
			while (cursor)
			{
				printf("cursor->id = %d\n", cursor->id);
				cursor = cursor->nextitem;
			}*/
			++y;
		}
	}
	lstaddobj(lstobj, obj);
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
	obj->normobj = NULL;
	return(obj);
}

bool			 	setmat(char **buffer, int *y, t_mat *mat)
{
	if (!ft_strcmp("MATERIAL", buffer[*y]))
	{
		if (buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3] && buffer[*y + 4] && buffer[*y + 5] && buffer[*y + 6] && buffer[*y + 7] && buffer[*y + 8] && buffer[*y + 9])
		{
			mat->diffuse = colorinit(ft_datoi(buffer[*y + 1]),
				ft_datoi(buffer[*y + 2]), ft_datoi(buffer[*y + 3]));
			mat->reflection = ft_datoi(buffer[*y + 4]);
			mat->brillance = ft_datoi(buffer[*y + 5]);
			mat->bump = ft_datoi(buffer[*y + 6]);
			mat->refraction = ft_datoi(buffer[*y + 7]);
			mat->transparency = ft_datoi(buffer[*y + 8]);
			mat->type = ft_datoi(buffer[*y + 9]);
			*y += 10;
		}
		if (mat->type > 0 && buffer[*y] && buffer[*y + 1] && buffer[*y + 2] && buffer[*y + 3])
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
	static t_obj* parentsave = NULL;

	if (parent)
		parentsave = parent;
	if (!ft_strcmp("NEGATIVE", buffer[*y]))
	{
		if (!(ft_strcmp(buffer[*y + 1], "SPHERE")))
		{
			*y += init_sphere(lstobj, &(*(buffer + *y + 1)), TRUE);
			(*lstobj)->normal = normalsphere;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "CONE")))
		{
			*y += init_cone(lstobj, &(*(buffer + *y + 1)), TRUE);
			(*lstobj)->normal = normalcone;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "CYLINDER")))
		{
			*y += init_cyl(lstobj, &(*(buffer + *y + 1)), TRUE);
			(*lstobj)->normal = normalcylinder;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "PLANE")))
		{
			*y += init_plane(lstobj, &(*(buffer + *y + 1)), TRUE);
			(*lstobj)->normal = normalplane;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "QUADRIC")))
		{
			*y += init_quadric(lstobj, &(*(buffer + *y + 1)), TRUE);
			(*lstobj)->normal = normalquadric;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "OBJECT")))
		{
			*y += init_object(lstobj, &(*(buffer + *y + 1)), TRUE);
		}
		(*lstobj)->pos = vectoradd(parentsave->pos, (*lstobj)->pos); // add something for direction too?
		(*lstobj)->isneg = TRUE;
		return (TRUE);
	}
	return (FALSE);
}

char  **char_tab_dup(char **tab)
{
	int i;
	int j;
	char **ret;

	i = 0;

	while (tab[i++])
	;
	ret = ft_memalloc(sizeof(char *) * (i + 1));
	ret[i] = NULL;
	j = -1;
	while (++j < i) {
		ret[j] = ft_strdup(tab[j]);
	}
	return ret;
}

void		init_cam(t_env *e, char **buffer)
{
	t_vec	up;
	t_vec	u;
	t_vec	v;

	t_vec	tmp_up;
	t_vec	tmp_vdir;
//	t_vec	rot_axis;
//	double	rot_angle;
	if (buffer[1] && buffer[2] && buffer[3])
		e->cam.eyepoint = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return;

	if (!ft_strcmp("LOOKAT", buffer[4]) && buffer[5] && buffer[6] && buffer[7])
	{
		e->cam.lookat = vectorinit(ft_datoi(buffer[5]), ft_datoi(buffer[6]), ft_datoi(buffer[7]));
		e->cam.vdir = vectorsub(e->cam.lookat, e->cam.eyepoint);
		vectornormalize(&e->cam.vdir);
	}
	else if (!ft_strcmp("ORIENTATION", buffer[4]) && buffer[5] && buffer[6] && buffer[7])
	{
		e->cam.vdir = vectorinit(ft_datoi(buffer[5]), ft_datoi(buffer[6]), ft_datoi(buffer[7]));
		vectornormalize(&e->cam.vdir);
	}
	else
		return;
/*	if (buffer)
	{
	e->cam.lookat = vectorinit(0, 0, -900);
	e->cam.eyepoint = vectorinit(0, 0, -1000);
	}*/

	//printf("e->cam.vdir.x = %g, e->cam.vdir.y = %g, e->cam.vdir.z = %g\n", e->cam.vdir.x, e->cam.vdir.y, e->cam.vdir.z);
	e->cam.lookat = vectorscale(SCREEN_EYE_DIST, e->cam.vdir); //temporary I hope

	//the tmp value represent an ordinary camera, that we'll rotate to fit the actual camera position and direction
	tmp_vdir = vectorinit(0, 0, 1);
	tmp_up = vectorinit(0, -1, 0); // why -1? Dunno, it works for now.

	/*rot_angle = acos(vectordot(tmp_vdir,  e->cam.vdir));
	rot_axis = vectorproduct(tmp_vdir,  e->cam.vdir);

	vectornormalize(&rot_axis);

	if (rot_axis.x || rot_axis.y || rot_axis.z)
	{
		up = vectorrotate(tmp_up, rot_axis, rot_angle);
	}
	else
	{
		up = tmp_up;
	}*/
	up = vectorinit(0, -1, 0);
//	rot_axis = vectorproduct(e->cam.vdir, up);
	//up = vectorrotate(e->cam.vdir, rot_axis, 90);
	//vectornormalize(&up);
//	printf("up.x = %g, up.y = %g, up.z = %g\n", up.x, up.y, up.z);
	u = vectorproduct(e->cam.vdir, up);
	printf("up.x = %g, up.y = %g, up.z = %g\n", up.x, up.y, up.z);
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
//	t_vec tmp = vectoradd(vectorscale(SCREEN_EYE_DIST, e->cam.vdir), e->cam.eyepoint);
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
//init object will set up the composed object rotation and position and allow it to be rendered
// by copying its component into the scene
int			init_object(t_obj **lstobj, char **buffer, bool neg)
{
	int 		y;
	t_obj		*obj;
	t_obj		*cursor;
	int 		id; // id of object(s) added to the scene

	obj = NULL;
	y = 2;
	id = 0;
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
		if (!ft_strcmp("ID", buffer[y]))
		{ //segfault
			if (buffer[y + 1])
			{
				id = ft_atoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else if (!ft_strcmp("POSITION", buffer[y]))
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
		else if (neg == FALSE && setnegative(buffer, &y, obj, &obj->nextneg));
		else if (neg == TRUE && setnegative(buffer, &y, NULL, lstobj));
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
		printf("extractobj\n");
	extractobj(lstobj, obj, id);
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



t_obj	*copyobj(t_obj *obj)
{
	t_obj *copy;

	copy = (t_obj*)malloc(sizeof(t_obj));
	copy->id = obj->id;
	//copy->id = obj->id;
	//copy->parent = obj->parent;
	copy->type = obj->type;
	copy->material = obj->material;
	copy->pos = obj->pos;
	copy->dir = obj->dir;
	copy->rad = obj->rad;
	copy->rad2 = obj->rad2;
	copy->height = obj->height;
	copy->alpha = obj->alpha;
	copy->quad = obj->quad;
	copy->isneg = obj->isneg;
	copy->rotation = obj->rotation;
	if (obj->nextneg)
	{
		copy->nextneg = copyobj(obj->nextneg);
	}
	else
	{
		copy->nextneg = NULL;
	}
	if (obj->nextslice)
	{
		copy->nextslice = copyobj(obj->nextslice);
	}
	else
	{
		copy->nextslice = NULL;
	}
	if (obj->nextitem)
	{
		copy->nextitem = copyobj(obj->nextitem);
	}
	else
	{
		copy->nextitem = NULL;
	}
	return (copy);
}

void 		rotateinnercomponents(t_obj *obj, t_obj *child)
{
	t_obj *cursor;

	cursor = child->nextneg;
	/*
	not taking into accound complex negative objects (need extra levels of recursion for this)
	*/
	while (cursor)
	{
		cursor->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir, vectoradd(cursor->dir, cursor->pos), obj->rotation);
		cursor->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir, vectoradd(obj->pos, cursor->pos), obj->rotation);
		cursor->dir = vectorsub(cursor->dir, cursor->pos);
		cursor = cursor->nextitem;
	}

	cursor = child->nextslice;
	while (cursor)
	{
		printf("obj->rotation2 = %g\n", obj->rotation);
		cursor->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir, vectoradd(cursor->dir, cursor->pos), obj->rotation);
		cursor->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir, vectoradd(obj->pos, cursor->pos), obj->rotation);
		cursor->dir = vectorsub(cursor->dir, cursor->pos);
		cursor->rotation = 0;
		cursor = cursor->nextslice;
	}
}

void		extractobj(t_obj **lstobj, t_obj *obj, int id)
{
	t_obj *tmp;
	t_obj *cursor;

	cursor = obj->nextchild;
	while (cursor)
	{
	//	printf("cursor->id = %d\n", cursor->id);
	//	printf("cursor->type =%d\n", cursor->type);
		if (cursor->nextchild)
		{
			if (cursor->type == 0)// if there is a child of another object containing children, add their coordinates
			{
				cursor->pos = vectoradd(cursor->pos, obj->pos);
			}
		//	printf("cursor->dir.x = %g, cursor->dir.y = %g, cursor->dir.z = %g\n", cursor->dir.x, cursor->dir.y, cursor->dir.z);
		//	cursor->rotation = obj->rotation;
		//	cursor->pos = obj->pos;
		//	cursor->dir = obj->dir;
			//sub composed objects inherit rotation, position and attributes for now, needs to be changed
			// Im using assigning parent values, but shouldn't I combine them with child values?
//			printf("innerextractobj\n");
			extractobj(lstobj, cursor, id);
		}
		else if (cursor->type > 0) //if type == 0, it only contains chidren
		{
			tmp = copyobj(cursor);
			tmp->id = id;
			t_vec worldfd = vectorinit(0, 0, -1); //forward of world, forward of object is dir

			double rot_angle;
			t_vec rot_axis;
			//get rotation between world and object direction
			vectornormalize(&rot_axis);

//			printf("tmp->id = %d\n", tmp->id);
//			printf("obj->rotation = %g\n", obj->rotation);

			tmp->dir = vectorpointrotatearoundaxis(obj->pos, obj->dir, vectoradd(tmp->dir, tmp->pos), obj->rotation);
//rotating the point (x,y,z) about the line through (a,b,c) with direction vector ⟨u,v,w⟩ (where u2 + v2 + w2 = 1) by the angle θ.
			tmp->pos = vectorpointrotatearoundaxis(obj->pos, obj->dir, vectoradd(obj->pos, tmp->pos), obj->rotation);
			tmp->dir = vectorsub(tmp->dir, tmp->pos);

			if (obj->rotation && (tmp->nextneg || tmp->nextslice))
			{
				printf("obj->rotation1 = %g\n", obj->rotation);
				rotateinnercomponents(obj, tmp);
			}

			vectornormalize(&tmp->dir);
			//tmp->dir = vectorrotate(tmp->pos, rot_axis, rot_angle);

			rot_angle = acos(vectordot(worldfd, obj->dir));
			rot_axis = vectorproduct(worldfd, obj->dir);
		//	tmp->dir = vectorrotate(tmp->dir, rot_axis, rot_angle);

			if (rot_axis.x || rot_axis.y || rot_axis.z) // if ROTATE
			{
				//	printf("ok\n");
				/*if (tmp->nextneg || tmp->nextslice)
				{
					printf("ok1\n");
					rotateinnercomponents(tmp, rot_axis, rot_angle);
				}*/
				tmp->pos = vectorrotate(tmp->pos, rot_axis, rot_angle);
				tmp->pos = vectoradd(tmp->pos, obj->pos);
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
