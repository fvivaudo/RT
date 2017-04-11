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

void			reset(t_env *e, int x, int y)
{
	t_vec viewplanepoint;

	viewplanepoint = vectoradd(
	vectoradd(e->cam.viewplanebottomleftpoint, vectorscale(x, e->cam.xincvector)),
	vectorscale(y, e->cam.yincvector));
	e->r.dir = vectorsub(viewplanepoint, e->cam.eyepoint);
	vectornormalize(&e->r.dir);
	e->id = 0;
	e->col.red = 0;
	e->col.green = 0;
	e->col.blue = 0;
	e->level = 0;
	e->coef = 1.0;
	e->r.initialstart = e->cam.eyepoint;
	e->r.start = e->cam.eyepoint;
	e->vdir = e->r.dir;
	e->crefraction = 1.0;
	e->transcoef = 0;
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

void		refract(t_env *e)
{
	double	ViewProjection = vectordot(e->r.dir, e->n);
	double coef = 1;
	double fCosThetaT;
	double 	fdensity1 = 1;
	double 	fdensity2 = e->cmat.refraction;
	double reflectance;
	double	fCosThetaI = fabs(ViewProjection);
	double fSinThetaI = sqrtf(1 - fCosThetaI * fCosThetaI);
	double fSinThetaT = (fdensity1 / fdensity2) * fSinThetaI;
	fSinThetaI = sqrt(1 - fCosThetaI * fCosThetaI);
	fSinThetaT = (fdensity1 / fdensity2) * fSinThetaI; 
	//printf("%f\n", fSinThetaI);
	if (fSinThetaT * fSinThetaT > 0.9999)
	{
 		reflectance = 1.0; // pure reflectance at grazing angle
		fCosThetaT = 0.0;
	}
	else
	{
	 	fCosThetaT = sqrtf(1 - fSinThetaT * fSinThetaT);
	 	double fReflectanceOrtho = (fdensity2 * fCosThetaT - fdensity1 * fCosThetaI ) / (fdensity2 * fCosThetaT + fdensity1 * fCosThetaI);
	 	fReflectanceOrtho = fReflectanceOrtho * fReflectanceOrtho;
	 
		double fReflectanceParal =(fdensity1 * fCosThetaT - fdensity2 * fCosThetaI ) / (fdensity1 * fCosThetaT + fdensity2 * fCosThetaI);

		fReflectanceParal = fReflectanceParal * fReflectanceParal;
		reflectance = 0.5 * (fReflectanceOrtho + fReflectanceParal);
	}

	double fRoulette = (1.0 / RAND_MAX) * rand();
	//printf("roulette  ==  %g            |||          reflectance == %g\n",fRoulette, reflectance);
	if (fRoulette < reflectance)
	{
	//	printf("lol\n");
	 // rays are either reflected, ..
	 	coef *= e->cmat.reflection;
	 	e->r.start = vectoradd(e->newstart, e->r.dir);
	 	e->r.dir = vectoradd(e->r.dir, vectorscale(e->cmat.reflection, e->n));
		vectornormalize(&e->r.dir);
	}
	else
	{
	  //..transmitted..
	//	printf("mdr\n");


		coef *= e->cmat.refraction;
		double fOldRefractionCoef = e->crefraction;
		e->obj->reversen = FALSE;
		if (e->obj->t[1] != DOESNOTEXIST)
		{
			e->crefraction = 1.0;
			e->obj->reversen = TRUE;						
		}
		else
		{
			e->crefraction = e->cmat.refraction;
		}

		e->r.start = vectoradd(e->newstart, e->r.dir);
		e->r.dir = vectoradd(e->r.dir,vectorscale(fCosThetaI, e->n));
		e->r.dir = vectorscale((fOldRefractionCoef / e->crefraction), e->r.dir);
		e->r.dir = vectoradd(e->r.dir, vectorscale(-fCosThetaT , e->n));
		vectornormalize(&e->r.dir);
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
	//ft_putendl("ok99");
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
	//	e.obj = NULL;
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
	e.transcoef = 1.0 - e.reflecoef;

	//ft_putendl("ok2");
	e.reflecoef *= e.cmat.reflection; //is it only reflection or diffusion too?
	e.transcoef *= e.cmat.transparency;

	//ft_putendl("ok3");
	originalcoef = e.coef /*  (1.0 - e.cmat.transparency)*/;
	tmpcoef1 = e.coef * e.reflecoef;
	tmpcoef2 = e.coef * e.transcoef;

	refracolor = colorinit(0, 0, 0);
	reflecolor = colorinit(0, 0, 0);
	//ft_putendl("ok4");

	++e.level;
	if (e.level < MAX_DEPTH_LEVEL && e.reflecoef > 0)
	{//only reflection
//	if (e.level != 0)
//	{
//		if (e.reflecoef)
//		if (e.transcoef)
//		printf("e.level = %d\n", e.level);
//	}
		//reflected ray = dir−2(dir⋅n )n
		e.coef = tmpcoef1;
		tmp = vectorscale(2 * vectordot(e.r.dir, e.n), e.n);
		e.r.dir = vectorsub(e.r.dir, tmp); // use temporary direction
		e.r.start = vectoradd(e.newstart, e.r.dir);
		vectornormalize(&e.r.dir);
		reflecolor = reflect_and_refract(e);
	}
	if (e.level < MAX_DEPTH_LEVEL && e.transcoef > 0)
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
	res.red = originalcoef * (e.reflecoef * reflecolor.red + e.transcoef * refracolor.red + e.col.red);
	res.green = originalcoef * (e.reflecoef * reflecolor.green + e.transcoef * refracolor.green + e.col.green);
	res.blue = originalcoef * (e.reflecoef * reflecolor.blue + e.transcoef * refracolor.blue + e.col.blue);
//	if (e.id == 2)
//		printf("res.red = %g, res.green = %g, res.blue = %g\n", res.red, res.green, res.blue);

	/*if ((e.coef > 0) && (e.level < MAX_DEPTH_LEVEL))
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
	return (res);
}

unsigned char	*update_img(t_env *e, int x, int y)
{
	static unsigned char	img[3 * WIDTH * HEIGHT];

	if (e)
	{
		img[(x + y * WIDTH) * 3 + 0] =
		(unsigned char)((e->col.red * C) < C ? e->col.red * C : C);
		img[(x + y * WIDTH) * 3 + 1] =
		(unsigned char)((e->col.green * C) < C ? e->col.green * C : C);
		img[(x + y * WIDTH) * 3 + 2] =
		(unsigned char)((e->col.blue * C) < C ? e->col.blue * C : C);
	}
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

t_obj	*copyallobj(t_obj *obj)
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
	copy->height = obj->height;
	copy->alpha = obj->alpha;
	copy->quad = obj->quad;
	copy->isneg = obj->isneg;
	copy->rotation = obj->rotation;
	if (obj->nextneg)
	{
		copy->nextneg = copyallobj(obj->nextneg);
	}
	if (obj->nextslice)
	{
		copy->nextslice = copyallobj(obj->nextslice);
	}
	if (obj->nextitem)
	{
		copy->nextitem = copyallobj(obj->nextitem);
	}
	return (copy);
}

/*t_cam copycam(t_cam cam)
{
	t_cam camcopy;

	camcopy.xincvector = cam.xincvector;
	camcopy.yincvector = cam.yincvector;
	camcopy.eyepoint = cam.eyepoint;
	camcopy.lookat = cam.lookat;
	camcopy.viewplanebottomleftpoint = cam.viewplanebottomleftpoint;
	camcopy.vdir = cam.vdir;

	return (camcopy);
}*/

static void		effect(t_env *new)
{
	double	tmpred;
	double	tmpgreen;
	double	tmpblue;

	
		tmpred = new->col.red;
		tmpgreen = new->col.green;
		tmpblue = new->col.blue;
		new->col.red = (tmpred + tmpgreen + tmpblue) * RED / 100;
		new->col.green = (tmpred + tmpgreen + tmpblue) * GREEN / 100;
		new->col.blue = (tmpred + tmpgreen + tmpblue) * BLUE / 100;
}


t_light	*copyalllights(t_light *light)
{
	t_light *copy;

	copy = (t_light*)malloc(sizeof(t_light));

	copy->pos = light->pos;
	copy->intensity = light->intensity;

	if (light->next)
	{
		copy->next = copyalllights(light->next);
	}
	return (copy);
}

void			*cast_ray_thread(void *e)
{
	t_env 	new;
	t_env 	*env;
	int 	interval;

	interval = ((t_thread_task*)e)->i;
	env = ((t_thread_task*)e)->arg;
	//printf("interval = %d\n", interval);

	new.obj = env->obj;
	new.cam = env->cam;
	new.lights = env->lights;
	new.x = 0;
	new.y = 0;
	get_img_pos(&new.x, &new.y, interval);

//	new.obj = copyallobj(*env->obj);
//	new.cam = env->cam;
//	new.lights = copyalllights(env->lights); // maybe copy a malloced version for each thread?
	
	while (1)
	{
		reset(&new, new.x, new.y);
		new.col = reflect_and_refract(new);
	
		if (EFFECT == 1)
			effect(&new);
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
		get_img_pos(&new.x, &new.y, interval);
	}
	return (NULL);
}

int				main(int ac, char **av)
{

	t_env			*original;
	t_env			*copy;
	int				fd;
	pthread_t		pth[MAX_THREAD];
	t_thread_task	arg;
	int				i;

	i = 0;
	if (ac != 2 || (fd = open(av[1], O_RDONLY)) <= -1 || !(original = readConfig(fd)))
	{
		//error message?
		return (0);
	}
	arg.i = 0;
		//ft_putendl("alive1");
	while (arg.i < MAX_THREAD)
	{
		++arg.i;
	//	ft_putendl("alive2");
		copy = (t_env*)malloc(sizeof(t_env));
		copy->obj = copyallobj(original->obj);
		copy->cam = original->cam;
		copy->lights = original->lights; // maybe copy a malloced version for each thread?
		arg.arg = (void*)copy;
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
	print_img(update_img(NULL, WIDTH - 1, HEIGHT - 1));
	return (0);
}
