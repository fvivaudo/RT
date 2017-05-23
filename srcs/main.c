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
switch(error)
{
    // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}

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

void		refract(t_ray *r, t_vec *n, t_mat *cmat, t_vec *inter_point, double *crefraction)
{
	double	ViewProjection = vectordot(r->dir, *n);
	double coef = 1;
	double fCosThetaT;
	double 	fdensity1 = *crefraction;
	double 	fdensity2 = cmat->refraction;
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
	 	coef *= cmat->reflection;
	 	r->start = vectoradd(*inter_point, r->dir);
	 	r->dir = vectoradd(r->dir, vectorscale(cmat->reflection, *n));
		vectornormalize(&r->dir);
	}
	else
	{
	  //..transmitted..
	//	printf("mdr\n");


		coef *= cmat->refraction;
		double fOldRefractionCoef = *crefraction;
		/*e->obj->reversen = FALSE;				//DoUBT
		if (e->obj->t[1] != DOESNOTEXIST)
		{
			e->crefraction = 1.0;
			e->obj->reversen = TRUE;						
		}
		else
		{*/
			*crefraction = cmat->refraction;
		//}

		r->start = vectoradd(*inter_point, r->dir);
		r->dir = vectoradd(r->dir,vectorscale(fCosThetaI, *n));
		r->dir = vectorscale((fOldRefractionCoef / *crefraction), r->dir);
		r->dir = vectoradd(r->dir, vectorscale(-fCosThetaT , *n));
		vectornormalize(&r->dir);
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
	//e.blue += 1;

	//e.cmat.reflection = 1.0;

	double originalcoef;
	double tmpcoef1;
	double tmpcoef2;

	double angle = cos(vectordot(e.r.dir, vectorscale(1.0, e.n)));
	double R0 = pow((e.crefraction - e.cmat.refraction) / (e.crefraction + e.cmat.refraction), 2);

	//shlick's approximation
	
	//double kr = R0 + (1.0 - R0) * pow((1.0 - angle), 5);

	//printf("angle  ==   %g ||| R0  ==    %g    |||   e.reflecoef  ==   %g\n", angle, R0, e.reflecoef);

	e.reflecoef = R0 + (1.0 - R0) * pow((1.0 - angle), 5) * (1.0 - e.cmat.transparency);
	e.transcoef = 1.0 - e.reflecoef; // how much light is reflected, computed by Fresnel equation 
//	e.reflecoef *= e.cmat.reflection; //is it only reflection or diffusion too?
//	e.transcoef *= e.cmat.transparency;

	//printf("e.transcoef == %g\n", e.transcoef);
	//printf(" e.reflecoef == %g\n", e.reflecoef);
	//printf(" sum == %g\n", e.reflecoef + e.transcoef);
	//if (e.transcoef)
	//printf(" e.transcoef == %g\n", e.transcoef);

	originalcoef = e.coef /*  (1.0 - e.cmat.transparency)*/;
	tmpcoef1 = e.coef * e.reflecoef;
	tmpcoef2 = e.coef * e.transcoef;

	refracolor = colorinit(0, 0, 0);
	reflecolor = colorinit(0, 0, 0);

	++e.level;
	if (e.level < MAX_DEPTH_LEVEL && e.cmat.reflection > 0)
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
	if (e.level < MAX_DEPTH_LEVEL && e.transcoef > 0/* && e.cmat.transparency > 0*/)
	{//only refraction
		e.coef = tmpcoef2;
//		refract(&e);
		refracolor = reflect_and_refract(e);
	//	printf("e.crefraction = %g\n", e.crefraction);
	//	printf("refracolor.red = %g, refracolor.green = %g, refracolor.blue = %g\n", refracolor.red, refracolor.green, refracolor.blue);

	//	e.coef = tmpcoef2;
	}
//	if (e.id == 2)
//	{
	//	printf("e.level = %d\n", e.level);
	//	printf("refracolor.red = %g, refracolor.green = %g, refracolor.blue = %g\n", refracolor.red, refracolor.green, refracolor.blue);
	//	printf("reflecolor.red = %g, reflecolor.green = %g, reflecolor.blue = %g\n", reflecolor.red, reflecolor.green, reflecolor.blue);
//	}
	if (e.cmat.transparency < 1.0 && e.cmat.reflection < 1.0) // transparency and reflection could be color specific, may implement later
	{
//		if (e.id == 2)
//			printf("ok\n");
		deal_shadow(&e);
	//	e.obj = NULL;
	}
/*	if (e.transcoef != 0.0)
	{
		res.red =  e.col.red;
		res.green =  e.col.green ;
		res.blue = e.col.blue;
	}	reflectionColor * kr + refractionColor * (1 - kr)
	else
	{*/
		res.red = reflecolor.red * e.reflecoef + refracolor.red * (1 - e.reflecoef) + e.col.red;
		res.green = reflecolor.green * e.reflecoef + refracolor.green * (1 - e.reflecoef) + e.col.green;
		res.blue = reflecolor.blue * e.reflecoef + refracolor.blue * (1 - e.reflecoef) + e.col.blue;
//	}
	
	//printf("res.blue = %g\n", res.blue);
//	if (e.id == 2)
//		printf("res.red = %g, res.green = %g, res.blue = %g\n", res.red, res.green, res.blue);
	//there is no shadow on the reflected surface, need to fix later
	
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

t_obj	*copyallobj(t_obj *obj)
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
	//copy->normobj = NULL;
	if (obj->nextneg)
	{
		copy->nextneg = copyallobj(obj->nextneg);
	}
	else
	{
		copy->nextneg = NULL;
	}
	if (obj->nextslice)
	{
		copy->nextslice = copyallobj(obj->nextslice);
	}
	else
	{
		copy->nextslice = NULL;
	}
	if (obj->nextitem)
	{
		copy->nextitem = copyallobj(obj->nextitem);
	}
	else
	{
		copy->nextitem = NULL;
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

void		effect(t_env *new, t_env *e)
{
	double	tmpred;
	double	tmpgreen;
	double	tmpblue;
//	printf("biss     === = =e->effect = %g, e->ered = %g, e->egreen = %g, e->eblue= %g\n", e->effect, e->ered, e->egreen, e->eblue);

	
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
/*
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
	//printf("effect %g, red %g , green %g ,  blue %g\n", env->col.effect, env->col.ered, env->col.egreen, env->col.eblue);

//	new.obj = copyallobj(*env->obj);
//	new.cam = env->cam;
//	new.lights = copyalllights(env->lights); // maybe copy a malloced version for each thread?
	
	while (1)
	{
		reset(&new, new.x, new.y);
		new.col = reflect_and_refract(new);
	
	//	printf("env->effect = %g, env->ered = %g, env->egreen = %g, env->eblue= %g\n", env->effect, env->ered, env->egreen, env->eblue);
		if (env->effect == 1)
			effect(&new, env);
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
}*/

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
	//	printf("len = %ld\n", fsize);
		res = ft_strext(res, srcs[i], 2);
		++i;
	}
	return (res);
}
/*
char *init_sources(char *name)
{
	char *r;
	FILE *f;
	long fsize;

	f = fopen(name, "rb");
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	r = malloc(fsize + 1);
	fread(r, fsize, 1, f);
	fclose(f);
	r[fsize] = 0;
	return (r);
}*/

void 	init_opencl(t_gputool *t, t_cl *cl)
{
	int 	ret;


	cl->source = init_sources(ft_strsplitspace("cl/vector.cl cl/normal.cl cl/solve.cl cl/perlin.cl cl/shadow.cl cl/test.cl"));

	printf("a %d\n",clGetDeviceIDs(NULL,CL_DEVICE_TYPE_GPU, 1, &cl->device_id, NULL));
	cl->context = clCreateContext(0, 1, &cl->device_id, NULL, NULL, NULL);

	cl->commands = clCreateCommandQueue(cl->context, cl->device_id, 0, NULL);
	cl->program = clCreateProgramWithSource(cl->context, 1, (const char **)&cl->source, NULL, &ret);
	printf("%s\n", getErrorString(ret));
	printf("%s\n", getErrorString(clBuildProgram(cl->program, 0, NULL, "-I includes", NULL, NULL)));
	//if (err == CL_BUILD_PROGRAM_FAILURE) {
	    // Determine the size of the log
	size_t log_size;
	clGetProgramBuildInfo(cl->program, cl->device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

	// Allocate memory for the log
	char *log = (char *) malloc(log_size);

	// Get the log
	clGetProgramBuildInfo(cl->program, cl->device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

	// Print the log
	printf("%s\n", log);
	//}
	int err;
	printf("sizeobj = %ld\n", sizeof(t_objgpu) * t->sizeobj);
	cl->kernel = clCreateKernel(cl->program, "actionn",&err);
	printf("#->c %d\n", err);
	cl->objs = clCreateBuffer(cl->context,  CL_MEM_READ_ONLY,  sizeof(t_objgpu) * t->sizeobj, NULL, NULL);
	cl->lights = clCreateBuffer(cl->context,  CL_MEM_READ_ONLY,  sizeof(t_lightgpu) * t->sizelight, NULL, NULL);
	printf("%d\n",clEnqueueWriteBuffer(cl->commands, cl->objs, CL_TRUE, 0, sizeof(t_objgpu) * t->sizeobj, t->gpu_in_objs, 0, NULL, NULL));
	printf("%d\n",clEnqueueWriteBuffer(cl->commands, cl->lights, CL_TRUE, 0, sizeof(t_lightgpu) * t->sizelight, t->gpu_in_lights, 0, NULL, NULL));
	printf("%d\n",clSetKernelArg(cl->kernel, 3, sizeof(cl_mem), &cl->objs));
	printf("%d\n",clSetKernelArg(cl->kernel, 4, sizeof(cl_mem), &cl->lights));

/*	clReleaseMemObject(cl->input);
    clReleaseMemObject(cl->output);
    clReleaseProgram(cl->program);
    clReleaseKernel(cl->kernel);
    clReleaseCommandQueue(cl->commands);
    clReleaseContext(cl->context);*/
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
//PREFERRED_WORK_GROUP_SIZE_MULTIPLE
void 	load_opencl(t_gputool *t, t_cl *cl, int count)
{
	int err;

	size_t returned_size = 0;
	size_t max_workgroup_size = 0;
//	size_t preffered_group_size = 0;

	err = clGetDeviceInfo(cl->device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_workgroup_size, &returned_size);
//	err = clGetDeviceInfo(cl->device_id, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &preffered_group_size, &returned_size);

	//printf("preffered size = %ld\n", returned_size);

	cl->count = count;
	printf("count = %d\n", count);
	clReleaseMemObject(cl->input);
	clReleaseMemObject(cl->output);
	cl->input = clCreateBuffer(cl->context, CL_MEM_READ_ONLY,  sizeof(t_ray) * cl->count, NULL, NULL);
	cl->output = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, sizeof(t_gpu_out) * cl->count, NULL, NULL);
	printf("%d\n",clEnqueueWriteBuffer(cl->commands, cl->input, CL_TRUE, 0, sizeof(t_ray) * cl->count, t->gpu_in, 0, NULL, NULL));
	printf("%d\n",clSetKernelArg(cl->kernel, 0, sizeof(cl_mem), &cl->input));
	printf("%d\n",clSetKernelArg(cl->kernel, 1, sizeof(cl_mem), &cl->output));
	printf("%d\n",clSetKernelArg(cl->kernel, 2, sizeof(unsigned int), &cl->count));
	printf("oui\n");
	printf("%d\n",clGetKernelWorkGroupInfo(cl->kernel, cl->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(cl->local), &cl->local, NULL));
	printf(" worker %lu\n", cl->local);
//	cl->local = greatestcommondivisor(returned_size, cl->count);
	cl->local = greatestmultiple(max_workgroup_size, cl->count);
//	printf("cl->local = %ld\n", cl->local);
	cl->global = cl->count;
	printf("start\n");
	err = clEnqueueNDRangeKernel(cl->commands, cl->kernel, 1, NULL, &cl->global, &cl->local, 0, NULL, NULL);
	printf("%s\n", getErrorString(err));
	clFinish(cl->commands);
	clEnqueueReadBuffer(cl->commands, cl->output, CL_TRUE, 0, sizeof(t_gpu_out) * cl->count, t->gpu_out, 0, NULL, NULL);
	printf("ended\n");
   // clReleaseMemObject(cl->output);
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

/*
get total number of objects/slice and negatives
*/
int 			getlistlength(t_obj *list)
{
	int size;
	t_obj *cursor;

	size = 0;
	while (list)
	{
		++size;
		cursor = list->nextneg;
		while (cursor)
		{
			++size;
			cursor = cursor->nextneg;
		}
		cursor = list->nextslice;
		while (cursor)
		{
			++size;
			cursor = cursor->nextslice;
		}
		list = list->nextitem;
	}

	return (size);
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

void			*cast_ray_thread(void *e)
{
	t_env 	*new;
	t_env 	*env;
	int 	interval;

	new = (t_env*)malloc(sizeof(t_env));
	interval = ((t_thread_task*)e)->i;
	env = ((t_thread_task*)e)->arg;
	printf("sizeof t_objgpu in cpu = %ld\n", sizeof(t_objgpu));
	//printf("interval = %d\n", interval);

//	ft_memcpy(&new->objgpu, &env->objgpu, sizeof(t_objgpu) * LIMIT_OBJ); // switch limit obj for size stored somewhere?
	new->obj = env->obj;
	new->cam = env->cam;
//	ft_memcpy(&new->lights, &env->lights, sizeof(t_light) * LIMIT_LIGHT);
	new->lights = env->lights;

	new->x = 0;
	new->y = 0;
	get_img_pos(&new->x, &new->y, interval);
	//printf("effect %g, red %g , green %g ,  blue %g\n", env->col.effect, env->col.ered, env->col.egreen, env->col.eblue);

//	new->obj = copyallobj(*env->obj);
//	new->cam = env->cam;
//	new->lights = copyalllights(env->lights); // maybe copy a malloced version for each thread?


	t_gputool gtool;
	gtool.sizeobj = getlistlength(env->obj) + 1;
	gtool.gpu_in = malloc(sizeof(t_ray) * HEIGHT * WIDTH);
	gtool.gpu_out = malloc(sizeof(t_gpu_out) * HEIGHT * WIDTH);
	gtool.gpu_in_objs = malloc(sizeof(t_objgpu) * gtool.sizeobj);
//	gtool.gpu_in_objs = memcpy(gtool.gpu_in_objs, env->objgpu, sizeof(t_objgpu) * LIMIT_OBJ);
	gtool.gpu_in_objs = fillgpuobj(gtool.gpu_in_objs, env->obj);


	gtool.sizelight = getlistlightlength(env->lights) + 1;
	gtool.gpu_in_lights = malloc(sizeof(t_light) * gtool.sizelight);
	gtool.gpu_in_lights = fillgpulights(gtool.gpu_in_lights, env->lights);

	gtool.pix_rendu = 0;

	while (1)
	{
		reset(new, new->x, new->y);

		t_ray ray;

		ray.initialstart = new->r.initialstart;
		ray.start = new->r.start;
		ray.dir = new->r.dir;
		ray.indirect = new->r.indirect;
		gtool.gpu_in[gtool.pix_rendu] = ray;
		ray.pixel_id = gtool.pix_rendu;
		gtool.pix_rendu++;
//		new->col = reflect_and_refract(new, &gtool);

	//	printf("env->effect = %g, env->ered = %g, env->egreen = %g, env->eblue= %g\n", env->effect, env->ered, env->egreen, env->eblue);
	/*
		if (env->effect == 1)
			effect(new, env);

		if (new->id != -1) //Ambient shading has to take place after every reflection took10 place
		{
			new->col.red += AMBIANT_SHADING * new->cmat.diffuse.red;
			new->col.green += AMBIANT_SHADING * new->cmat.diffuse.green;
			new->col.blue += AMBIANT_SHADING * new->cmat.diffuse.blue;
			//exposure/ saturation
			new->col.red = 1.0 - exp(new->col.red * EXPOSURE);
			new->col.blue = 1.0 - exp(new->col.blue * EXPOSURE);
			new->col.green = 1.0 - exp(new->col.green * EXPOSURE);
		}
		*/

		//update_img(new, new->x, new->y);
		if (new->x >= WIDTH && new->y >= HEIGHT-1)
			break;
		get_img_pos(&new->x, &new->y, interval);

	}
//cast ray
	//measure number of secondary ray
	//malloc array of coef for secondary rays, and rays
	//repeat
	int level = 0;
	int sizerays = HEIGHT * WIDTH;
	t_cl cl;

	env->col = colorinit(0, 0, 0);
	init_opencl(&gtool, &cl);

	t_sray_info *inforays = NULL;
	while (level < MAX_DEPTH_LEVEL)
	{
		int sizenewray; //determine the size of the new array of rays sent to gpu
		t_ray *newrays; //new table of rays, size determined by sizenewray
		t_sray_info *newinforays; //new table of rays, size determined by sizenewray
		t_gpu_out *newout;

		newout = NULL;
		load_opencl(&gtool, &cl, sizerays);
		printf("gtool.pix_rendu = %d\n", gtool.pix_rendu);
	/*	for (int i = 0; i < gtool.sizeobj; ++i)
		{
			printf("gtool.gpu_in_objs[i].pos.x = %g gtool.gpu_in_objs[i].pos.y = %g gtool.gpu_in_objs[i].pos.z = %g\n", gtool.gpu_in_objs[i].pos.x, gtool.gpu_in_objs[i].pos.y, gtool.gpu_in_objs[i].pos.z);
			printf("gtool.gpu_in_objs[i].dir.x = %g gtool.gpu_in_objs[i].dir.y = %g gtool.gpu_in_objs[i].dir.z = %g\n", gtool.gpu_in_objs[i].dir.x, gtool.gpu_in_objs[i].dir.y, gtool.gpu_in_objs[i].dir.z);
			printf("gtool.gpu_in_objs[i].type = %d\n", gtool.gpu_in_objs[i].type);
			printf("gtool.gpu_in_objs[i].type_obj = %d\n", gtool.gpu_in_objs[i].type_obj);
		}*/
		printf("openok\n");

		sizenewray = 0;

		for (int g = 0; g < sizerays; ++g)
		{
			if (gtool.gpu_in_objs[gtool.gpu_out[g].id].material.transparency)
				++sizenewray;
			if (gtool.gpu_in_objs[gtool.gpu_out[g].id].material.reflection)
				++sizenewray;
		}

		double *coef = NULL;
		if (sizenewray > 0)
		{
			coef = (double*)malloc(sizeof(double) * sizerays);
			newrays = (t_ray*)malloc(sizeof(t_ray) * sizenewray);
			newout = (t_gpu_out*)malloc(sizeof(t_gpu_out) * sizenewray);
			newinforays = (t_sray_info*)malloc(sizeof(t_sray_info) * sizenewray);

			int gbis = 0;
			for (int g = 0; g < sizerays; ++g)
			{
					//printf("env->col.red = %g, env->col.green = %g env->col.blue = %g\n", env->col.red, env->col.green, env->col.blue );
				t_color			reflecolor;
				t_color			refracolor;
				t_vec			tmp;

				t_mat 			*cmat;
				t_ray 			*ray;
				double 			crefraction;
				t_vec 			inter_point;
				t_vec 			normal;

				normal = gtool.gpu_out[g].normal;
				ray = &gtool.gpu_out[g].r;
				cmat = &gtool.gpu_in_objs[gtool.gpu_out[g].id].material;
				inter_point = vectoradd(ray->start, vectorscale(gtool.gpu_out[g].t, ray->dir));
				if (level)
				{
					coef[g] = inforays[g].coef;
					crefraction = inforays[g].previousrefractionindex;
				}
				else
				{
					coef[g] = 1;
					crefraction = 1;
				}

			//	if (e.id == 2)
			//		printf("e.cmat.transparency = %g\n", e.cmat.transparency);
				//e.blue += 1;

				//e.cmat.reflection = 1.0;

				double reflecoef;
				double transcoef;

				double tmpcoef1;
				double tmpcoef2;

				double angle = cos(vectordot(ray->dir, vectorscale(1.0, gtool.gpu_out[g].normal)));
				double R0 = pow((crefraction - cmat->refraction) / (crefraction + cmat->refraction), 2);

				//shlick's approximation
				
				//double kr = R0 + (1.0 - R0) * pow((1.0 - angle), 5);

				reflecoef = R0 + (1.0 - R0) * pow((1.0 - angle), 5) * (1.0 - cmat->transparency);
				transcoef = 1.0 - reflecoef; // how much light is reflected, computed by Fresnel equation 

				tmpcoef1 = coef[g] * reflecoef;
				tmpcoef2 = coef[g] * transcoef;

				refracolor = colorinit(0, 0, 0);
				reflecolor = colorinit(0, 0, 0);

				if (level < MAX_DEPTH_LEVEL && cmat->reflection > 0)
				{//only reflection

				//	printf("g = %d\n", g);
				//	printf("gbis = %d\n", gbis);
				//	printf("sizenewray = %d\n", sizenewray);
					//coef = 1; //tmp
					tmp = vectorscale(2 * vectordot(ray->dir, gtool.gpu_out[g].normal), gtool.gpu_out[g].normal);
					ray->dir = vectorsub(ray->dir, tmp); // use temporary direction
					ray->start = vectoradd(inter_point, ray->dir);
					vectornormalize(&ray->dir);
					newinforays[gbis].coef = tmpcoef1;
					newinforays[gbis].previousrefractionindex = 1;
					newrays[gbis++] = *ray;
					coef[g] -= tmpcoef1;
					//reflecolor = reflect_and_refract(e);
				}
				if (level < MAX_DEPTH_LEVEL && transcoef > 0 && cmat->transparency > 0)
				{//only refraction
			//		ft_putendl("ok0");
					refract(ray, &normal, cmat, &inter_point, &crefraction);
			//		ft_putendl("ok1");
					newinforays[gbis].coef = tmpcoef2;
			//		ft_putendl("ok2");
					newinforays[gbis].previousrefractionindex = crefraction;
			//		ft_putendl("ok3");
					newrays[gbis++] = *ray;
			//		ft_putendl("ok4");
					coef[g] -= tmpcoef2;
				}
			//		printf("gbis = %d\n", gbis);
			}
		}
		if (coef)
		{
			for (int g = 0; g < sizerays; ++g)
			{
				update_img(&gtool.gpu_out[g].color, gtool.gpu_in[g].pixel_id, coef[g]);
			}
		}
		else
		{
			for (int g = 0; g < sizerays; ++g)
			{
				update_img(&gtool.gpu_out[g].color, gtool.gpu_in[g].pixel_id, 1);
			}
		}
		++level;
		sizerays = sizenewray;
		free(gtool.gpu_in);
		gtool.gpu_in = newrays;
		free(gtool.gpu_out);
		gtool.gpu_out = newout;
		free(inforays);
		inforays = newinforays;
		free(coef);
		if (sizerays == 0)
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

	i = 0;
	if (ac != 2 || (fd = open(av[1], O_RDONLY)) <= -1 || !(original = readConfig(fd)))
	{
		//error message?
		return (0);
	}

	printf("confok\n");

    
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

//	++arg.i;
//	cast_ray_thread((void*)&arg);
//	e->available_ressource = 1;
//	e->y = 0;

	while (i < arg.i)
	{
		pthread_join(pth[i], NULL);
		++i;
	}
	print_img(update_img(NULL, 0, 0));
	return (0);
}
