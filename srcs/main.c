/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2016/12/23 11:23:12 by fvivaudo         ###   ########.fr       */
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

t_vec	bump_mapping(t_env *e)
{
	t_vec	tmp;
	t_vec	new_n;
	double	temp;

	tmp = vectorscale(0.1, e->newstart);
	double noiseCoefx = noise(tmp.x, tmp.y, tmp.z);
	double noiseCoefy = noise(tmp.y, tmp.z, tmp.x);
	double noiseCoefz = noise(tmp.z, tmp.x, tmp.y);
	new_n.x = (1.0f -  e->cmat.bump ) * e->n.x +  e->cmat.bump * noiseCoefx; 
	new_n.y = (1.0f -  e->cmat.bump ) * e->n.y +  e->cmat.bump * noiseCoefy; 
	new_n.z = (1.0f -  e->cmat.bump ) * e->n.z +  e->cmat.bump * noiseCoefz; 
	temp = vectordot(new_n ,new_n);
	if (temp == 0.0f)
		return (e->n);
	temp = 1.0 / sqrtf(temp);
	new_n = vectorscale(temp, new_n);
	vectornormalize(&new_n);
	return (new_n);
}

void	blinn_phong(t_env *e, t_vec lightray_dir)
{
	double			tmp;
	double			blinnterm;
	t_vec			blinnDir;

	blinnDir = vectorsub(lightray_dir, e->r.dir);
	tmp = sqrtf(vectordot(blinnDir, blinnDir));
	if (tmp)
	{
		blinnDir = vectorscale((1 / tmp), blinnDir);
		tmp = vectordot(blinnDir, e->n);
		blinnterm = tmp ? tmp : 0;
		blinnterm = e->cmat.brillance * pow(blinnterm, SPEC_POW) * e->coef;
		e->col.red += e->cmat.brillance * blinnterm;
		e->col.green += e->cmat.brillance * blinnterm;
		e->col.blue += e->cmat.brillance * blinnterm;
	}
}

/*for each light in the scene
**{
**	if the light is not in shadow of another object
**	{
**		add this light contribution to computed color;
**	}
**}*/
int		deal_shadow(t_env *e)
{
	t_ray			lightray;
	t_light			*tmplight;
	double			tmpdot;
	double			distancetolight;

	tmplight = e->lights;
		
	while (tmplight)
	{
		if (e->cmat.bump)
		{
			e->n = bump_mapping(e);
		}

		e->clight = *tmplight;
		tmplight = tmplight->next;
		//distance between intersection point and light
		e->dist = vectorsub(e->clight.pos, e->newstart);
		distancetolight = vectormagnitude(e->dist);
//		printf("Checkpoint0\n");
/*		printf("e->newstart.x = %g, e->newstart.y = %g, e->newstart.z = %g\n", e->newstart.x, e->newstart.y, e->newstart.z);
		printf("e->clight.pos.x = %g, e->clight.pos.y = %g, e->clight.pos.z = %g\n", e->clight.pos.x, e->clight.pos.y, e->clight.pos.z);
		printf("e->n.x = %g, e->n.y = %g, e->n.z = %g\n", e->n.x, e->n.y, e->n.z);
		printf("e->dist.x = %g, e->dist.y = %g, e->dist.z = %g\n", e->dist.x, e->dist.y, e->dist.z);
		printf("e->t = %g\n", e->t);*/

		//if the vector dot is negative, then both vectors are going in opposite directions
		vectornormalize(&e->dist);
		if ((tmpdot = vectordot(e->n, e->dist)) <= 0.0f)
			continue;
		e->t = vectormagnitude(e->dist);
		if (e->t <= 0.0f)
			continue; // delete later, eventually

		//Check if there is something between point and light
		//limits light range, or does it?
	//	lightray.dir = vectorscale((1 / e->t), e->dist); //alternative to normalization?
		lightray.dir = e->dist;
		lightray.start = vectoradd(e->newstart, e->dist);
		//lightray.start = vectoradd(e->newstart, lightray.dir);
		// the object risks a colision with itself, which is quite problematic
		// 2 solutions :
		// -forbid colision detection with itself 
		// -launch the ray a small distance away from the actual intersection point (CURRENT)
		//printf("intersection\n");
	//	printf("intersection\n");
		/*if (intersection(e, &lightray, -1))
		{
			continue;
		}*/

		double light = 1;
		if ((light = computeshadow(e, &lightray, light, distancetolight)) == 0.0) // 1 is light, is it ok?
		{
			continue;
		}
	//	printf("light = %g\n", light);
	//	printf("intersection2\n");

	//	lambert diffusion
		e->lambert = tmpdot * e->coef;
		e->col.red += e->lambert * e->clight.intensity.red * e->cmat.diffuse.red * light;
		e->col.green += e->lambert * e->clight.intensity.green * e->cmat.diffuse.green * light;
		e->col.blue += e->lambert * e->clight.intensity.blue * e->cmat.diffuse.blue * light;

		//check collision with objects between object and light

		// Blinn Phong model
		blinn_phong(e, lightray.dir);
	}
	if (e->col.red == 0 && e->col.green == 0 && e->col.red == 0)
		return (FALSE);
	return (TRUE);
	//Ambient lighting

	//	printf("e->red = %g, e->green = %g, e->blue = %g\n", e->red, e->green, e->blue);
}

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
	return (res);
/*	if ((e.coef > 0) && (e.level < MAX_DEPTH_LEVEL))
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

	}*/
}

/*
** send a ray.
** if the ray meets something, deal with it's shadow
** else, generate another  ray
*/
void			cast_ray(t_env *e)
{
	t_vec			tmp;
	t_obj			*res;
	//t_color			col;
	e->t = MAX_RANGE;
	res = computeray(e);
	if (!res)
		return;
	e->cmat = res->material;

	deal_shadow(e);
	//e->blue += 1;

	e->coef *= e->cmat.reflection;


	//reflected ray = dir−2(dir⋅n )n
/*	if (e->coef > 0)
	{
		col = reflect_and_refract(*e);
		e->red = col.red;
		e->green = col.green;
		e->blue = col.blue;
	}*/

	tmp = vectorscale(2 * vectordot(e->r.dir, e->n), e->n);
	e->r.dir = vectorsub(e->r.dir, tmp);
	e->r.start = vectoradd(e->newstart, e->r.dir);
	vectornormalize(&e->r.dir);
	if ((e->coef > 0) && (e->level < MAX_DEPTH_LEVEL))
	{
		++e->level;
		cast_ray(e);
	}
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
	//ft_putendl("image update");
	return (img);
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
//		ft_putendl(buffer_gnl);
		buffer_line = ft_strsplitspace(buffer_gnl);
		if (buffer_line[0])
		{
			if (!(ft_strcmp(buffer_line[0], "SPHERE")))
			{
				init_sphere(e, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "CONE")))
			{
				init_cone(e, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "CYLINDER")))
			{
				init_cyl(e, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "PLANE")))
			{
				init_plane(e, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "QUADRIC")))
			{
				init_quadric(e, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "LIGHT")))
			{
				init_light(e, buffer_line);
			}
			else if (!(ft_strcmp(buffer_line[0], "CAMERA")))
			{
				init_cam(e, buffer_line);
			}
		}
		free(buffer_gnl);
		ft_doubletabfree(&buffer_line);
		++e->id;
	}
	close(fd);
	return (e);
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
		//pthread_mutex_lock(&env->mutex);
		/*while (!env->available_ressource)
		{
			pthread_cond_wait(&env->cond, &env->mutex);
		}*/
		//printf("beyond the lock\n");
		new.cam = env->cam;
		new.lights = env->lights; // maybe copy a malloced version for each thread?
		new.obj = env->obj; // maybe copy a malloced version for each thread?

		get_img_pos(&new.x, &new.y, interval);
		//pthread_mutex_unlock(&env->mutex);
		//env->available_ressource = 1;
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
	//	printf("new.red = %g, new.green = %g, new.blue = %g\n", new.red, new.green, new.blue);
		update_img(&new, new.x, new.y);
		//printf("new->x = %d, new->y = %d\n", new.x, new.y);
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
