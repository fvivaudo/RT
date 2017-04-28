#include <rtv1.h>
#include <stdio.h>
#include <math.h>

int				init_negcyl(t_obj *obj, char **buffer)
{
	int			y;
	t_neg		neg;
	t_mat		material;

	y = 4;

	if (buffer[1] && buffer[2] && buffer[3])
		neg.pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[y] != NULL && ft_strcmp(buffer[y], "NEGATIVE"))
	{
		if (setmat(buffer, &y, &material));
		else if (!ft_strcmp("RADIUS", buffer[y]))
		{
			if (buffer[y + 1])
			{
				neg.rad = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else if (setorient(buffer, &y, (t_obj*)&neg));
		else if (setslice(buffer, &y, (t_obj*)&neg));
		else if (!ft_strcmp("HEIGHT", buffer[y]))
		{
			if (buffer[y + 1])
			{
				neg.height = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
				return (y);
		}
		else
			++y;
	}
	neg.type = TYPE_CYLINDER;
	neg.material = material;
	//neg.id = e->id;
	*obj = addneg(*obj, neg);
	return (y);
}

int		init_negcone(t_obj *obj, char **buffer)
{
	int			y;
	t_neg		neg;
	t_mat		material;

	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		neg.pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[y] != NULL && ft_strcmp(buffer[y], "NEGATIVE"))
	{
		if (setmat(buffer, &y, &material));
		else if (!ft_strcmp("ANGLE", buffer[y]))
		{
			if (buffer[y + 1])
			{
				neg.rad = ft_datoi(buffer[y + 1]);
				neg.alpha = neg.rad * (M_PI / 180);
				y += 2;
			}
			else
				return (y);
		}
		else if (setorient(buffer, &y, (t_obj*)&neg));
		else if (setslice(buffer, &y, (t_obj*)&neg));
		else
			++y;
	}
	neg.type = TYPE_CONE;
	neg.material = material;
	*obj = addneg(*obj, neg);
	return (y);
}

int		init_negsphere(t_obj *obj, char **buffer)
{
	int			y;
	t_neg		neg;
	t_mat		material;

	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		neg.pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
		return (1);
	while (buffer[y] != NULL && ft_strcmp(buffer[y], "NEGATIVE"))
	{
		if (setmat(buffer, &y, &material));
		else if (setslice(buffer, &y, (t_obj*)&neg));
		else if (!ft_strcmp("RADIUS", buffer[y]))
		{
			if (buffer[y + 1])
			{
				neg.rad = ft_datoi(buffer[y + 1]);
				y += 2;
			}
			else
			{
				printf("ret1\n");
				return (y);
			}
		}
		else
		{
			++y;
		}
	}
	neg.type = TYPE_SPHERE;
	neg.material = material;
	//neg.pos = neg.pos;
	*obj = addneg(*obj, neg);
	return (y);
}

int		init_negquadric(t_obj *obj, char **buffer)
{
	int			y;
	t_neg		neg;
	t_mat		material;
	t_quadric	quad;

	y = 4;
	if (buffer[1] && buffer[2] && buffer[3])
		neg.pos = vectorinit(ft_datoi(buffer[1]), ft_datoi(buffer[2]), ft_datoi(buffer[3]));
	else
	{
		return (0);
	}
	while (buffer[y] != NULL && ft_strcmp(buffer[y], "NEGATIVE"))
	{
		if (setmat(buffer, &y, &material));
		else if (setorient(buffer, &y, (t_obj *)&neg));
		else if (setslice(buffer, &y, (t_obj *)&neg));
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
	rot_angle = acos(vectordot(y_axis, neg.dir));
	rot_axis = vectorproduct(y_axis, neg.dir);

	//neg.quad = quadricrotate(quad, vectorproduct(neg.dir, vectorinit(0, 1, 0)), acos(vectordot(vectorinit(0, 1, 0), neg.dir)));
	if (rot_angle)
	{
		neg.quad = quadricrotate(quad, rot_axis, rot_angle, neg.pos);
	}
	else
	{
		neg.quad = quadricrotate(quad, y_axis, 0, neg.pos);
	}
//	neg.quad = quadricrotate(quad, vectorinit(1, 0, 0), 90);
//	neg.quad = quad;
	neg.type = TYPE_QUADRIC;
	neg.material = material;
	//neg.pos = neg.pos;
	*obj = addneg(*obj, neg);
	return (y);
}

bool				setnegative(char **buffer, int *y, t_obj *parent)
{
	if (!ft_strcmp("NEGATIVE", buffer[*y]))
	{
		if (!(ft_strcmp(buffer[*y + 1], "SPHERE")))
		{
			*y += init_negsphere(parent, &(*(buffer + *y + 1)));
		//	(*lstobj)->normal = normalsphere;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "CONE")))
		{
			*y += init_negcone(parent, &(*(buffer + *y + 1)));
		//	(*lstobj)->normal = normalcone;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "CYLINDER")))
		{
			*y += init_negcyl(parent, &(*(buffer + *y + 1)));
		//	(*lstobj)->normal = normalcylinder;
		}
		else if (!(ft_strcmp(buffer[*y + 1], "QUADRIC")))
		{
			*y += init_negquadric(parent, &(*(buffer + *y + 1)));
		//	(*lstobj)->normal = normalquadric;
		}

		int i;

		i = 0;
		while (parent->nextneg[i].set == TRUE && i < LIMIT_NEG)
		{
			++i;
		}
		/*else if (!(ft_strcmp(buffer[*y + 1], "OBJECT")))
		{
			*y += init_object(lstobj, &(*(buffer + *y + 1)), TRUE);
		}*/
		//(*lstobj)->pos = vectoradd(parentsave->pos, (*lstobj)->pos); // add something for direction too?
		return (TRUE);
	}
	return (FALSE);
}
