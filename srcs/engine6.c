/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/17 15:40:03 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>


int 			getlistlightlength(t_light *list)
{
	int size;

	size = 0;
	while (list)
	{
		++size;
		list = list->next;
	}

	return (size);
}

t_lightgpu 			*fillgpulights(t_lightgpu *array, t_light *list)
{
	int i;

	i = 0;
	while (list)
	{
		ft_memcpy(&array[i], list, sizeof(t_lightgpu));
		array[i].set = TRUE;
		list = list->next;
		++i;
	}
	array[i].set = FALSE;
	return (array);
}

t_gputool 	init_gtool(t_env *env)
{
	t_gputool gtool;
	gtool.sizeobj = getlistlength(env->obj) + 1;
	gtool.gpu_in = malloc(sizeof(t_ray) * HEIGHT * WIDTH);
	gtool.gpu_out = malloc(sizeof(t_gpu_out) * HEIGHT * WIDTH);
	gtool.gpu_in_objs = malloc(sizeof(t_objgpu) * gtool.sizeobj);
	gtool.gpu_in_objs = fillgpuobj(gtool.gpu_in_objs, env->obj);
	gtool.sizelight = getlistlightlength(env->lights) + 1;
	gtool.gpu_in_lights = malloc(sizeof(t_light) * gtool.sizelight);
	gtool.gpu_in_lights = fillgpulights(gtool.gpu_in_lights, env->lights);
	gtool.pix_rendu = 0;
	return (gtool);
}

void 			cast_ray_thread_func(t_env *new, t_gputool *gtool,
	int *interval)
{
	t_ray ray;

	while (1)
	{
		reset(new, new->x, new->y);
		ray.initialstart = new->r.initialstart;
		ray.start = new->r.start;
		ray.dir = new->r.dir;
		ray.indirect = new->r.indirect;
		gtool->gpu_in[gtool->pix_rendu] = ray;
		ray.pixel_id = gtool->pix_rendu;
		gtool->pix_rendu++;
		if (new->x >= WIDTH && new->y >= HEIGHT-1)
			break;
		get_img_pos(&new->x, &new->y, *interval);
	}
}

t_env 			*cast_init_new(t_env *env)
{
	t_env *new;
	new = (t_env*)malloc(sizeof(t_env));
	new->obj = env->obj;
	new->cam = env->cam;
	new->lights = env->lights;
	new->x = 0;
	new->y = 0;
	return (new);
}
