/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:35:29 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

void			reset(t_env *e, int x, int y)
{
	t_vec viewplanepoint;

	viewplanepoint = vectoradd(
	vectoradd(e->cam.viewplanebottomleftpoint, vectorscale(x, e->cam.xincvector)),
	vectorscale(y, e->cam.yincvector));
	e->r.dir = vectorsub(viewplanepoint, e->cam.eyepoint);
	vectornormalize(&e->r.dir);
	e->id = -1;
	e->col.red = 0;
	e->col.green = 0;
	e->col.blue = 0;
	e->level = 0;
	e->coef = 1.0;
	e->r.start = e->cam.eyepoint;
	e->vdir = e->r.dir;
	e->crefraction = 1.0;
	e->refracoef = 0;
	e->reflecoef = 0;
//	e->r.start.x = e->eyepoint.x + x;
//	e->r.start.y = e->eyepoint.y + y;
}


/*t_color	marble(t_env *e)
{

}*/

//color perturbation, to implement later, maybe.
/*		t_vec output;
		double noiseCoef = 0;
		e->cmat.diffuse2 = colorinit(1, 1, 0);

		for (int level = 1; level < 10; level++)
		{
			noiseCoef += (1.0 / level) * fabs(noise(level * 0.05 * e->newstart.x, level * 0.05 * e->newstart.y, level * 0.05 * e->newstart.z));
		};
		noiseCoef = 0.5 * sin((e->newstart.x + e->newstart.y) * 0.05 + noiseCoef) + 0.5;
		output = vectoradd(
		vectorscale(noiseCoef, *(t_vec*)&e->cmat.diffuse)
		,vectorscale((1.0 - noiseCoef), *(t_vec*)&e->cmat.diffuse2));

		e->cmat.diffuse.red = output.x;
		e->cmat.diffuse.green = output.y;
		e->cmat.diffuse.blue = output.z;*/



void			refract(t_env *e)
{
	double ndoti, two_ndoti, a, b, b2, D2;
	ndoti = vectordot(e->n, e->r.dir);
	// r = relative refractence index
	double r = e->crefraction / e->cmat.refraction;
	double invr = e->cmat.refraction / e->crefraction;

	if (ndoti >= 0.0)
	{
		b = r;
		b2 = r * r;
	}
	else
	{
		b = invr;
		b2 = invr * invr;
	}
	D2 = 1.0 - b2 * (1.0 - (ndoti * ndoti));

	if (D2 >= 0.0)
	{
		if (ndoti >= 0.0)
			a = b * ndoti - sqrt(D2);
		else
			a = b * ndoti + sqrt(D2);
//		printf("ndoti = %g\n", ndoti);
//		printf("e->r.dir1.x = %g, e->r.dir1.y = %g, e->r.dir1.z = %g\n", e->r.dir.x, e->r.dir.y, e->r.dir.z);
		e->r.dir.x = a * e->n.x - b * e->r.dir.x;
		e->r.dir.y = a * e->n.y - b * e->r.dir.y;
		e->r.dir.z = a * e->n.z - b * e->r.dir.z;
//		printf("e->r.dir2.x = %g, e->r.dir2.y = %g, e->r.dir2.z = %g\n", e->r.dir.x, e->r.dir.y, e->r.dir.z);
		e->r.start = vectoradd(e->newstart, e->r.dir);
		e->crefraction = e->cmat.refraction; //solve later.
	}
	else // total internal reflection
	{
		two_ndoti = ndoti + ndoti;
		e->r.dir.x = two_ndoti * e->n.x - e->r.dir.x;
		e->r.dir.y = two_ndoti * e->n.y - e->r.dir.y;
		e->r.dir.z = two_ndoti * e->n.z - e->r.dir.z;
		e->r.start = vectoradd(e->newstart, e->r.dir);
	}
}

t_color			reflect_and_refract(t_env e)
{
	t_color			reflecolor;
	t_color			refracolor;
	t_color			res;
	t_vec			tmp;
	t_obj			*collide;

	e.t = MAX_RANGE;
	collide = computeray(&e);
	if (!collide)
	{
		res = colorinit(0, 0, 0);
		return (res);
	}
	e.cmat = collide->material;

//	if (e.id == 2)
//		printf("e.cmat.transparency = %g\n", e.cmat.transparency);
	if (e.cmat.transparency < 1.0 && e.cmat.reflection < 1.0) // transparency and reflection could be color specific, may implement later
	{
//		if (e.id == 2)
//			printf("ok\n");
		deal_shadow(&e);
	}//there is no shadow on the reflected surface, need to fix later
	//e.blue += 1;

	//e.cmat.reflection = 1.0;

	double originalcoef;
	double tmpcoef1;
	double tmpcoef2;

	double angle = cos(vectordot(e.r.dir, e.n));
	//ft_putendl("ok0");
	double R0 = pow((e.crefraction - e.cmat.refraction) / (e.crefraction + e.cmat.refraction), 2);

	//ft_putendl("ok1");
	//shlick's approximation
	e.reflecoef = R0 + (1.0 - R0) * pow((1.0 - angle), 5);
	e.refracoef = 1.0 - e.reflecoef;

	//ft_putendl("ok2");
	e.reflecoef *= e.cmat.reflection; //is it only reflection or diffusion too?
	e.refracoef *= e.cmat.refraction;

	//ft_putendl("ok3");
	originalcoef = e.coef;
	tmpcoef1 = e.coef * e.reflecoef;
	tmpcoef2 = e.coef * e.refracoef;

	refracolor = colorinit(0, 0, 0);
	reflecolor = colorinit(0, 0, 0);
	//ft_putendl("ok4");

	++e.level;
	if (e.level < MAX_DEPTH_LEVEL && e.reflecoef > 0)
	{//only reflection
		//reflected ray = dir−2(dir⋅n )n
		e.coef = tmpcoef1;
		tmp = vectorscale(2 * vectordot(e.r.dir, e.n), e.n);
		e.r.dir = vectorsub(e.r.dir, tmp); // use temporary direction
		e.r.start = vectoradd(e.newstart, e.r.dir);
		vectornormalize(&e.r.dir);
		reflecolor = reflect_and_refract(e);
	}
	if (e.level < MAX_DEPTH_LEVEL && e.refracoef > 0)
	{//only refraction
		e.coef = tmpcoef2;
	//ft_putendl("ok6");
	//	printf("e.r.dir1.x = %g, e.r.dir1.y = %g, e.r.dir1.z = %g\n", e.r.dir.x, e.r.dir.y, e.r.dir.z);
		refract(&e);
	//	printf("e.r.dir2.x = %g, e.r.dir2.y = %g, e.r.dir2.z = %g\n", e.r.dir.x, e.r.dir.y, e.r.dir.z);
	//ft_putendl("ok7");
		refracolor = reflect_and_refract(e);
	//	printf("e.crefraction = %g\n", e.crefraction);
	//	printf("refracolor.red = %g, refracolor.green = %g, refracolor.blue = %g\n", refracolor.red, refracolor.green, refracolor.blue);

	//ft_putendl("ok8");
	//	e.coef = tmpcoef2;
	}
//	if (e.id == 2)
//	{
	//	printf("e.level = %d\n", e.level);
	//	printf("refracolor.red = %g, refracolor.green = %g, refracolor.blue = %g\n", refracolor.red, refracolor.green, refracolor.blue);
	//	printf("reflecolor.red = %g, reflecolor.green = %g, reflecolor.blue = %g\n", reflecolor.red, reflecolor.green, reflecolor.blue);
//}
	res.red = originalcoef * (e.reflecoef * reflecolor.red + e.refracoef * refracolor.red + e.col.red);
	res.green = originalcoef * (e.reflecoef * reflecolor.green + e.refracoef * refracolor.green + e.col.green);
	res.blue = originalcoef * (e.reflecoef * reflecolor.blue + e.refracoef * refracolor.blue + e.col.blue);
//	if (e.id == 2)
//		printf("res.red = %g, res.green = %g, res.blue = %g\n", res.red, res.green, res.blue);

	if ((e.coef > 0) && (e.level < MAX_DEPTH_LEVEL))
	{
		++e.level;
		//reflected ray = dir−2(dir⋅n )n
		tmp = vectorscale(2 * vectordot(e.r.dir, e.n), e.n);
		e.r.dir = vectorsub(e.r.dir, tmp);
		e.r.start = vectoradd(e.newstart, e.r.dir);
		vectornormalize(&e.r.dir);
		reflect_and_refract(e);
	}
	else
	{

	}
	return (res);
}

unsigned char	*update_img(t_env *e, int x, int y)
{
	static unsigned char	img[3 * WIDTH * HEIGHT];

	img[(x + y * WIDTH) * 3 + 0] =
	(unsigned char)((e->col.red * C) < C ? e->col.red * C : C);
	img[(x + y * WIDTH) * 3 + 1] =
	(unsigned char)((e->col.green * C) < C ? e->col.green * C : C);
	img[(x + y * WIDTH) * 3 + 2] =
	(unsigned char)((e->col.blue * C) < C ? e->col.blue * C : C);
	return (img);
}

void			get_img_pos(int *x, int *y, int inter)
{
	*x += inter;
	while (*x > WIDTH)
	{
		*x -= WIDTH;
		++(*y);
		if (*y >= HEIGHT)
			*x = WIDTH;
	}
}

void			*cast_ray_thread(void *e)
{
	t_env 	new;
	t_env 	*env;
	int 	interval;

	interval = ((t_thread_task*)e)->i;
	env = ((t_thread_task*)e)->arg;
	//printf("interval = %d\n", interval);

	get_img_pos(&new.x, &new.y, interval);
	while (1)
	{
		new.cam = env->cam;
		new.lights = env->lights; // maybe copy a malloced version for each thread?
		new.obj = env->obj; // maybe copy a malloced version for each thread?

		get_img_pos(&new.x, &new.y, interval);
		reset(&new, new.x, new.y);
//		while ((new.coef > 0) && (new.level < MAX_DEPTH_LEVEL))
//		{
			//cast_ray(&new);
			new.col = reflect_and_refract(new);
//			++new.level;
//		}
		if (new.id != -1) //Ambient shading has to take place after every reflection took place
		{
			new.col.red += AMBIANT_SHADING * new.cmat.diffuse.red;
			new.col.green += AMBIANT_SHADING * new.cmat.diffuse.green;
			new.col.blue += AMBIANT_SHADING * new.cmat.diffuse.blue;
			//exposure/ saturation
			new.col.red = 1.0 - exp(new.col.red * EXPOSURE);
			new.col.blue = 1.0 - exp(new.col.blue * EXPOSURE);
			new.col.green = 1.0 - exp(new.col.green * EXPOSURE);
		}
		update_img(&new, new.x, new.y);
		if (new.x >= WIDTH && new.y >= HEIGHT)
		{
		//	pthread_cond_broadcast(&e->cond, &e->mutex);
			break;
		}
	}
	return (NULL);
}

int				main(int ac, char **av)
{
	t_env			*e;
	int				fd;
	pthread_t		pth[MAX_THREAD];
	t_thread_task	arg;
	int				i;

	i = 0;
	if (ac != 2 || (fd = open(av[1], O_RDONLY)) <= -1 || !(e = readConfig(fd)))
	{
		//error message?
		return (0);
	}

	arg.i = 0;
	arg.arg = (void*)e;
		//ft_putendl("alive1");
	while (arg.i < MAX_THREAD)
	{
		++arg.i;
	//	ft_putendl("alive2");
		pthread_create(&pth[arg.i - 1], NULL, cast_ray_thread, (void *)&arg);
		usleep(100); //better way to do things?
	}
//	++arg.i;
//	cast_ray_thread((void*)&arg);
	//	ft_putendl("alive3");
//	e->available_ressource = 1;
//	e->y = 0;

	while (i < arg.i)
	{
		pthread_join(pth[i], NULL);
		++i;
	}
	print_img(update_img(e, WIDTH - 1, HEIGHT - 1));
	return (0);
}
