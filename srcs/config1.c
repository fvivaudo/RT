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
