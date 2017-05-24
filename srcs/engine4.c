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
