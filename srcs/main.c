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

const char *getErrorString(cl_int error)
{
	return "non";
}

void			reset(t_env *e, int x, int y)
{
	t_vec viewplanepoint;

	viewplanepoint = vectoradd(
	vectoradd(e->cam.viewplanebottomleftpoint,
		vectorscale(x, e->cam.xincvector)),
	vectorscale(y, e->cam.yincvector));
	e->r.dir = vectorsub(viewplanepoint, e->cam.eyepoint);
	vectornormalize(&e->r.dir);
	e->id = -1;
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
}

typedef struct s_nrefract
{
	double	ViewProjection;
	double coef;
	double fCosThetaT;
	double 	fdensity1;
	double 	fdensity2;
	double reflectance;
	double	fCosThetaI;
	double fSinThetaI;
	double fSinThetaT;
}				t_nrefract;

typedef struct s_cast
{
	t_ray *r;
	t_vec *n;
	t_mat *cmat;
	t_vec *inter_point;
	double *crefraction;
}				t_cast;

t_nrefract 		refract_init(t_cast o)
{
	t_nrefract u;

	u.ViewProjection = vectordot(o.r->dir, *o.n);
	u.coef = 1;
	u.fdensity1 = *o.crefraction;
	u.fdensity2 = o.cmat->refraction;
	u.fCosThetaI = fabs(u.ViewProjection);
	u.fSinThetaI = sqrtf(1 - u.fCosThetaI * u.fCosThetaI);
	u.fSinThetaT = (u.fdensity1 / u.fdensity2) * u.fSinThetaI;
	return (u);
}

void 	refract_func(t_cast *o, t_nrefract *u, double *fOldRefractionCoef,
	double *fRoulette)
{
		*fRoulette = (1.0 / RAND_MAX) * rand();
		if (*fRoulette < u->reflectance)
		{
		 	u->coef *= o->cmat->reflection;
		 	o->r->start = vectoradd(*o->inter_point, o->r->dir);
		 	o->r->dir = vectoradd(o->r->dir, vectorscale(o->cmat->reflection,
				*o->n));
			vectornormalize(&o->r->dir);
		}
		else
		{
			u->coef *= o->cmat->refraction;
			*fOldRefractionCoef = *o->crefraction;
			o->r->start = vectoradd(*o->inter_point, o->r->dir);
			o->r->dir = vectoradd(o->r->dir,vectorscale(u->fCosThetaI, *o->n));
			o->r->dir = vectorscale((*fOldRefractionCoef / *o->crefraction),
				o->r->dir);
			o->r->dir = vectoradd(o->r->dir, vectorscale(-u->fCosThetaT ,
				*o->n));
			vectornormalize(&o->r->dir);
		}
}

void		refract(t_cast o)
{
	t_nrefract u;
	double fRoulette = (1.0 / RAND_MAX) * rand();
	double fReflectanceOrtho;
	double fReflectanceParal;
	double fOldRefractionCoef;

	u = refract_init(o);
	if (u.fSinThetaT * u.fSinThetaT > 0.9999)
	{
 		u.reflectance = 1.0;
		u.fCosThetaT = 0.0;
	}
	else
	{
	 	u.fCosThetaT = sqrtf(1 - u.fSinThetaT * u.fSinThetaT);
	 	fReflectanceOrtho = COSCALC / COSCALC;
	 	fReflectanceOrtho = fReflectanceOrtho * fReflectanceOrtho;
		fReflectanceParal = COSSCALC / COSSCALC;
		fReflectanceParal = fReflectanceParal * fReflectanceParal;
		u.reflectance = 0.5 * (fReflectanceOrtho + fReflectanceParal);
	}
	refract_func(&o, &u, &fOldRefractionCoef, &fRoulette);
}

typedef struct 	s_raus
{
	t_color			reflecolor;
	t_color			refracolor;
	t_color			res;
	t_vec			tmp;
	t_obj			*collide;
	double originalcoef;
	double tmpcoef1;
	double tmpcoef2;
	double angle ;
	double R0 ;
}				t_raus;

void 			reflect_and_refract_func(t_raus *o, t_env *e)
{
	e->cmat = o->collide->material;
	o->angle = cos(vectordot(e->r.dir, vectorscale(1.0, e->n)));
	o->R0 = pow((e->crefraction - e->cmat.refraction) / (e->crefraction +
		e->cmat.refraction), 2);
	e->reflecoef = o->R0 + (1.0 - o->R0) * pow((1.0 - o->angle), 5) * (1.0 -
		e->cmat.transparency);
	e->transcoef = 1.0 - e->reflecoef;
	o->originalcoef = e->coef;
	o->tmpcoef1 = e->coef * e->reflecoef;
	o->tmpcoef2 = e->coef * e->transcoef;
	o->refracolor = colorinit(0, 0, 0);
	o->reflecolor = colorinit(0, 0, 0);
	++e->level;
}
void 			reflect_and_refract_func2(t_raus *o, t_env *e)
{
	o->res.red = o->reflecolor.red * e->reflecoef + o->refracolor.red *
		(1 - e->reflecoef) + e->col.red;
	o->res.green = o->reflecolor.green * e->reflecoef + o->refracolor.green
		* (1 - e->reflecoef) + e->col.green;
	o->res.blue = o->reflecolor.blue * e->reflecoef + o->refracolor.blue
		* (1 - e->reflecoef) + e->col.blue;
}

void 			reflect_and_refract_func3(t_raus *o, t_env *e)
{
	e->coef = o->tmpcoef1;
	o->tmp = vectorscale(2 * vectordot(e->r.dir, e->n), e->n);
	e->r.dir = vectorsub(e->r.dir, o->tmp);
	e->r.start = vectoradd(e->newstart, e->r.dir);
}

t_color			reflect_and_refract(t_env e)
{
	t_raus o;

	e.t = MAX_RANGE;
	o.collide = computeray(&e);
	if (!o.collide)
	{
		o.res = colorinit(0, 0, 0);
		return (o.res);
	}
	reflect_and_refract_func(&o, &e);
	if (e.level < MAX_DEPTH_LEVEL && e.cmat.reflection > 0)
	{
		reflect_and_refract_func3(&o, &e);
		vectornormalize(&e.r.dir);
		o.reflecolor = reflect_and_refract(e);
	}
	if (e.level < MAX_DEPTH_LEVEL && e.transcoef > 0)
	{
		e.coef = o.tmpcoef2;
		o.refracolor = reflect_and_refract(e);
	}
	if (e.cmat.transparency < 1.0 && e.cmat.reflection < 1.0)
		deal_shadow(&e);
	reflect_and_refract_func2(&o, &e);
	return (o.res);
}

unsigned char	*update_img(t_color *col, int pixel_id, double coef)
{
	static unsigned char	img[3 * WIDTH * HEIGHT];

	if (pixel_id > WIDTH * HEIGHT || pixel_id < 0)
		return (img);
	if (col)
	{
		col->red = col->red > C ? C : col->red;
		img[pixel_id * 3 + 0] += (unsigned char)(col->red * C * coef);
		col->green = col->green > C ? C : col->green;
		img[pixel_id * 3 + 1] += (unsigned char)(col->green * C * coef);
		col->blue = col->blue > C ? C : col->blue;
		img[pixel_id * 3 + 2] += (unsigned char)(col->blue * C * coef);
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

t_obj 	*copyallobj_func(t_obj *obj)
{
	t_obj *copy;

	copy = (t_obj*)malloc(sizeof(t_obj));
	copy->id = obj->id;
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
	copy->type_obj = obj->type_obj;
	return (copy);
}

t_obj	*copyallobj(t_obj *obj)
{
	t_obj *copy;

	copy = copyallobj_func(obj);
	if (obj->nextneg)
		copy->nextneg = copyallobj(obj->nextneg);
	else
		copy->nextneg = NULL;
	if (obj->nextslice)
		copy->nextslice = copyallobj(obj->nextslice);
	else
		copy->nextslice = NULL;
	if (obj->nextitem)
		copy->nextitem = copyallobj(obj->nextitem);
	else
		copy->nextitem = NULL;
	return (copy);
}

void		effect(t_env *new, t_env *e)
{
	double	tmpred;
	double	tmpgreen;
	double	tmpblue;

	tmpred = new->col.red;
	tmpgreen = new->col.green;
	tmpblue = new->col.blue;
	new->col.red = (tmpred + tmpgreen + tmpblue) * e->ered / 100;
	new->col.green = (tmpred + tmpgreen + tmpblue) * e->egreen / 100;
	new->col.blue = (tmpred + tmpgreen + tmpblue) * e->eblue / 100;
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

char *init_sources(char **srcs)
{
	char *res;
	FILE *f;
	int i;
	long fsize;

	res = ft_strdup("");
	i = 0;
	while (srcs[i] != NULL)
	{
		f = fopen(srcs[i], "rb");
		fseek(f, 0, SEEK_END);
		fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		srcs[i] = malloc(fsize + 1);
		fread(srcs[i], fsize, 1, f);
		fclose(f);
		srcs[i][fsize] = 0;
		res = ft_strext(res, srcs[i], 2);
		++i;
	}
	return (res);
}

void init_opencl_func(t_gputool *t, t_cl *cl)
{
	int err;

	cl->kernel = clCreateKernel(cl->program, "actionn",&err);
	cl->objs = clCreateBuffer(cl->context,  CL_MEM_READ_ONLY,
		sizeof(t_objgpu) * t->sizeobj, NULL, NULL);
	cl->lights = clCreateBuffer(cl->context,  CL_MEM_READ_ONLY,
		sizeof(t_lightgpu) * t->sizelight, NULL, NULL);
	clEnqueueWriteBuffer(cl->commands, cl->objs, CL_TRUE, 0,
		sizeof(t_objgpu) * t->sizeobj, t->gpu_in_objs, 0, NULL, NULL);
	clEnqueueWriteBuffer(cl->commands, cl->lights, CL_TRUE, 0,
		sizeof(t_lightgpu) * t->sizelight, t->gpu_in_lights, 0, NULL, NULL);
	clSetKernelArg(cl->kernel, 3, sizeof(cl_mem), &cl->objs);
	clSetKernelArg(cl->kernel, 4, sizeof(cl_mem), &cl->lights);
}

void 	init_opencl(t_gputool *t, t_cl *cl)
{
	int 	ret;

	cl->source = init_sources(ft_strsplitspace("cl/vector.cl \
	cl/normal.cl cl/solve.cl\
	 cl/perlin.cl cl/shadow.cl cl/test.cl"));
	clGetDeviceIDs(NULL,CL_DEVICE_TYPE_GPU, 1, &cl->device_id, NULL);
	cl->context = clCreateContext(0, 1, &cl->device_id, NULL, NULL, NULL);
	cl->commands = clCreateCommandQueue(cl->context, cl->device_id, 0, NULL);
	cl->program = clCreateProgramWithSource(cl->context, 1,
		(const char **)&cl->source, NULL, &ret);
	clBuildProgram(cl->program, 0, NULL, "-I includes", NULL, NULL);
	size_t log_size;
	clGetProgramBuildInfo(cl->program, cl->device_id,
		CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	char *log = (char *) malloc(log_size);
	clGetProgramBuildInfo(cl->program, cl->device_id,
		CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
	init_opencl_func(t, cl);
}

int greatestcommondivisor(int a, int b)
{
    return (b == 0 ? a : greatestcommondivisor(b, a % b));
}

int greatestmultiple(int maxmultiple, int value)
{
	int i;

	i = maxmultiple;
	while (i > 0)
	{
		if ((value % i) == 0)
			return (i);
		--i;
	}
	return (i);
}

void 	load_opencl_func(t_gputool *t, t_cl *cl, int max_workgroup_size)
{
	int err;

	clSetKernelArg(cl->kernel, 1, sizeof(cl_mem), &cl->output);
	clSetKernelArg(cl->kernel, 2, sizeof(unsigned int), &cl->count);
	clGetKernelWorkGroupInfo(cl->kernel, cl->device_id,
		CL_KERNEL_WORK_GROUP_SIZE, sizeof(cl->local), &cl->local, NULL);
	cl->local = greatestmultiple(max_workgroup_size, cl->count);
	cl->global = cl->count;
	err = clEnqueueNDRangeKernel(cl->commands, cl->kernel,
		1, NULL, &cl->global, &cl->local, 0, NULL, NULL);
	getErrorString(err);
	clFinish(cl->commands);
	clEnqueueReadBuffer(cl->commands, cl->output, CL_TRUE, 0,
		sizeof(t_gpu_out) * cl->count, t->gpu_out, 0, NULL, NULL);
}

void 	load_opencl(t_gputool *t, t_cl *cl, int count)
{
	int err;
	size_t returned_size;
	size_t max_workgroup_size;

	returned_size = 0;
	max_workgroup_size = 0;
	err = clGetDeviceInfo(cl->device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE,
		sizeof(size_t), &max_workgroup_size, &returned_size);
	cl->count = count;
	clReleaseMemObject(cl->input);
	clReleaseMemObject(cl->output);
	cl->input = clCreateBuffer(cl->context, CL_MEM_READ_ONLY,
		sizeof(t_ray) * cl->count, NULL, NULL);
	cl->output = clCreateBuffer(cl->context, CL_MEM_READ_WRITE,
		sizeof(t_gpu_out) * cl->count, NULL, NULL);
	clEnqueueWriteBuffer(cl->commands, cl->input, CL_TRUE, 0,
		sizeof(t_ray) * cl->count, t->gpu_in, 0, NULL, NULL);
	clSetKernelArg(cl->kernel, 0, sizeof(cl_mem), &cl->input);
	load_opencl_func(t, cl, max_workgroup_size);
}

void 	free_opencl(t_cl *cl)
{
    clReleaseProgram(cl->program);
    clReleaseKernel(cl->kernel);
    clReleaseCommandQueue(cl->commands);
    clReleaseContext(cl->context);
}

t_objgpu 			*fillgpuobj(t_objgpu *array, t_obj *list)
{
	int i;
	t_objgpu tmp;
	t_obj *cursor;

	tmp.type = 0;
	i = 0;
	while (list)
	{
		ft_memcpy(&array[i], list, sizeof(t_objgpu));
		cursor = list->nextslice;
		while (cursor)
		{
			++i;
			ft_memcpy(&array[i], cursor, sizeof(t_objgpu));
			cursor = cursor->nextslice;
		}
		cursor = list->nextneg;
		while (cursor)
		{
			++i;
			ft_memcpy(&array[i], cursor, sizeof(t_objgpu));
			cursor = cursor->nextitem;
		}
		list = list->nextitem;
		++i;
	}
	ft_memcpy(&array[i], &tmp, sizeof(t_objgpu));
	return (array);
}

typedef struct s_gll
{
	int size;
	t_obj *cursor;
}				t_gll;

int 			getlistlength(t_obj *list)
{
	t_gll g;

	g.size = -1;
	while (list && (++g.size != -1))
	{
		g.cursor = list->nextneg;
		while (g.cursor)
		{
			++g.size;
			g.cursor = g.cursor->nextneg;
		}
		g.cursor = list->nextslice;
		while (g.cursor)
		{
			++g.size;
			g.cursor = g.cursor->nextslice;
		}
		list = list->nextitem;
	}
	return (g.size);
}

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

typedef struct s_cc
{
	t_env 	*new;
	t_env 	*env;
	int 	interval;
	t_gputool gtool;
	int level;
	int sizerays;
	double reflecoef;
	double transcoef;
	double tmpcoef1;
	double tmpcoef2;
	double angle ;
	double R0 ;
	t_color			reflecolor;
	t_color			refracolor;
	t_vec			tmp;
	t_mat 			*cmat;
	t_ray 			*ray;
	double 			crefraction;
	t_vec 			inter_point;
	t_vec 			normal;
	t_cl cl;
	t_sray_info *inforays;
	int sizenewray;
	t_ray *newrays;
	t_sray_info *newinforays;
	t_gpu_out *newout;
	double *coef;
	int gbis;
	t_cast par;
}				t_cc;

void 			cast_func0(t_cc *c, t_env *e)
{
	c->interval = ((t_thread_task*)e)->i;
	c->env = ((t_thread_task*)e)->arg;
	c->new = cast_init_new(c->env);
	get_img_pos(&c->new->x, &c->new->y, c->interval);
	c->gtool = init_gtool(c->env);
	cast_ray_thread_func(c->new, &c->gtool, &c->interval);
	c->level = 0;
	c->sizerays = HEIGHT * WIDTH;
	c->env->col = colorinit(0, 0, 0);
	init_opencl(&c->gtool, &c->cl);
	c->inforays = NULL;
}


void 			cast_func1(t_cc *c)
{
	int g;

	g = -1;
	c->newout = NULL;
	load_opencl(&c->gtool, &c->cl, c->sizerays);
	c->sizenewray = 0;
	while (++g < c->sizerays)
	{
		if (c->gtool.gpu_in_objs[c->gtool.gpu_out[g].id].material.transparency)
			++c->sizenewray;
		if (c->gtool.gpu_in_objs[c->gtool.gpu_out[g].id].material.reflection)
			++c->sizenewray;
	}
	c->coef = NULL;
}

void 			cast_func3(t_cc *c, int g)
{
	c->normal = c->gtool.gpu_out[g].normal;
	c->ray = &c->gtool.gpu_out[g].r;
	c->cmat = &c->gtool.gpu_in_objs[c->gtool.gpu_out[g].id].material;
	c->inter_point = vectoradd(c->ray->start,
		vectorscale(c->gtool.gpu_out[g].t, c->ray->dir));
	if (c->level)
	{
		c->coef[g] = c->inforays[g].coef;
		c->crefraction = c->inforays[g].previousrefractionindex;
	}
	else
	{
		c->coef[g] = 1;
		c->crefraction = 1;
	}
}

void 			cast_func6(t_cc *c, int g)
{
	c->angle = cos(vectordot(c->ray->dir, vectorscale(1.0,
		c->gtool.gpu_out[g].normal)));
	c->R0 = pow((c->crefraction - c->cmat->refraction) / (c->crefraction
		+ c->cmat->refraction), 2);
	c->reflecoef = c->R0 + (1.0 - c->R0) * pow((1.0 - c->angle), 5) * (1.0 -
		c->cmat->transparency);
	c->transcoef = 1.0 - c->reflecoef;
	c->tmpcoef1 = c->coef[g] * c->reflecoef;
	c->tmpcoef2 = c->coef[g] * c->transcoef;
	c->refracolor = colorinit(0, 0, 0);
	c->reflecolor = colorinit(0, 0, 0);
}

void 			cast_func4(t_cc *c, int g)
{
	cast_func6(c, g);
	if (c->level < MAX_DEPTH_LEVEL && c->cmat->reflection > 0)
	{
		c->tmp = vectorscale(2 * vectordot(c->ray->dir,
			c->gtool.gpu_out[g].normal), c->gtool.gpu_out[g].normal);
		c->ray->dir = vectorsub(c->ray->dir, c->tmp);
		c->ray->start = vectoradd(c->inter_point, c->ray->dir);
		vectornormalize(&c->ray->dir);
		c->newinforays[c->gbis].coef = c->tmpcoef1;
		c->newinforays[c->gbis].previousrefractionindex = 1;
		c->newrays[c->gbis++] = *c->ray;
		c->coef[g] -= c->tmpcoef1;
	}
}

void 			cast_func2(t_cc *c, int g)
{
	c->coef = (double*)malloc(sizeof(double) * c->sizerays);
	c->newrays = (t_ray*)malloc(sizeof(t_ray) * c->sizenewray);
	c->newout = (t_gpu_out*)malloc(sizeof(t_gpu_out) * c->sizenewray);
	c->newinforays = (t_sray_info*)malloc(sizeof(t_sray_info) * c->sizenewray);
	c->gbis = 0;
	while(++g < c->sizerays)
	{
		cast_func3(c, g);
		cast_func4(c, g);
		if (c->level < MAX_DEPTH_LEVEL && c->transcoef > 0 &&
			 c->cmat->transparency > 0)
		{
			c->par.r = c->ray;
			c->par.n = &c->normal;
			c->par.cmat = c->cmat;
			c->par.inter_point = &c->inter_point;
			c->par.crefraction = &c->crefraction;
			refract(c->par);
			c->newinforays[c->gbis].coef = c->tmpcoef2;
			c->newinforays[c->gbis].previousrefractionindex = c->crefraction;
			c->newrays[c->gbis++] = *c->ray;
			c->coef[g] -= c->tmpcoef2;
		}
	}
}

void 			cast_func5(t_cc *c, int g)
{
	if (c->coef)
	{
		while(++g < c->sizerays)
			update_img(&c->gtool.gpu_out[g].color,
				c->gtool.gpu_in[g].pixel_id, c->coef[g]);
	}
	else
	{
		g = -1;
		while(++g < c->sizerays)
			update_img(&c->gtool.gpu_out[g].color,
				c->gtool.gpu_in[g].pixel_id, 1);
	}
}


void			*cast_ray_thread(void *e)
{
	t_cc c;

	cast_func0(&c, e);
	while (c.level < MAX_DEPTH_LEVEL)
	{
		cast_func1(&c);
		if (c.sizenewray > 0)
			cast_func2(&c, -1);
		cast_func5(&c, -1);
		++c.level;
		c.sizerays = c.sizenewray;
		free(c.gtool.gpu_in);
		c.gtool.gpu_in = c.newrays;
		free(c.gtool.gpu_out);
		c.gtool.gpu_out = c.newout;
		free(c.inforays);
		c.inforays = c.newinforays;
		free(c.coef);
		if (c.sizerays == 0)
			return (NULL);
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
