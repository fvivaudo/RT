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
