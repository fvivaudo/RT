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
