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

int				main(int ac, char **av)
{

	t_env			*original;
	t_env			*copy;
	int				fd;
	pthread_t		pth[MAX_THREAD];
	t_thread_task	arg;
	int				i;
	readConfig2(av[1]);
	i = 0;
	if (ac != 2 || (fd = open("confage", O_RDONLY)) <= -1 || !(original = readConfig(fd)))
	{
		//error message?
		return (0);
	}
	arg.i = 0;
	while (arg.i < MAX_THREAD)
	{
		++arg.i;
		copy = (t_env*)malloc(sizeof(t_env));
		copy->effect = original->effect;
		copy->ered = original->ered;
		copy->egreen = original->egreen;
		copy->eblue = original->eblue;

		copy->obj = copyallobj(original->obj);
		copy->cam = original->cam;
		copy->lights = original->lights; // maybe copy a malloced version for each thread?
		arg.arg = (void*)copy;
		pthread_create(&pth[arg.i - 1], NULL, cast_ray_thread, (void *)&arg);
		usleep(100); //better way to do things?
	}
	while (i < arg.i)
	{
		pthread_join(pth[i], NULL);
		++i;
	}
	print_img(update_img(NULL, 0, 0));
	return (0);
}
