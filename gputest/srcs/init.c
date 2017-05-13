/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/11 20:10:57 by momartin          #+#    #+#             */
/*   Updated: 2017/01/11 20:10:59 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gputest.h"

void init_sources(t_env *e){
	FILE *f = fopen("srcs/test.cl", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	CL->source = malloc(fsize + 1);
	fread(CL->source, fsize, 1, f);
	fclose(f);
	CL->source[fsize] = 0;
}

void 	init_opencl(t_env *e){
	CL->count = SIZE;
	for (int i=0;i < SIZE;i++){
		CL->data[i] = 25;
	}
	printf("%d\n",clGetDeviceIDs(NULL,CL_DEVICE_TYPE_GPU, 1, &CL->device_id, NULL));
	CL->context = clCreateContext(0, 1, &CL->device_id, NULL, NULL, NULL);
	CL->commands = clCreateCommandQueue(CL->context, CL->device_id, 0, NULL);
	CL->program = clCreateProgramWithSource(CL->context, 1, (const char **)&CL->source, NULL, NULL);
	printf("%d\n",clBuildProgram(CL->program, 0, NULL, NULL, NULL, NULL));
	int err;
	CL->kernel = clCreateKernel(CL->program, "actionn",&err);
	printf("#->%d\n", err);
	CL->input = clCreateBuffer(CL->context,  CL_MEM_READ_ONLY,  sizeof(float)*CL->count, NULL, NULL);
	CL->output = clCreateBuffer(CL->context, CL_MEM_WRITE_ONLY, sizeof(float)*CL->count, NULL, NULL);
	printf("%d\n",clEnqueueWriteBuffer(CL->commands, CL->input, CL_TRUE, 0, sizeof(float)*CL->count, CL->data, 0, NULL, NULL));
	printf("%d\n",clSetKernelArg(CL->kernel, 0, sizeof(cl_mem), &CL->input));
	printf("%d\n",clSetKernelArg(CL->kernel, 1, sizeof(cl_mem), &CL->output));
	printf("%d\n",clSetKernelArg(CL->kernel, 2, sizeof(unsigned int), &CL->count));
	printf("oui\n");
	printf("%d\n",clGetKernelWorkGroupInfo(CL->kernel, CL->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(CL->local), &CL->local, NULL));
	printf(" worker %lu\n", CL->local);
	CL->local = 800;
	CL->global = CL->count;


	clEnqueueNDRangeKernel(CL->commands, CL->kernel, 1, NULL, &CL->global, &CL->local, 0, NULL, NULL);
	clFinish(CL->commands);
	clEnqueueReadBuffer( CL->commands, CL->output, CL_TRUE, 0, sizeof(float) * CL->count, CL->result, 0, NULL, NULL);
	for (int i=0;i<SIZE;i++){
		//printf("--> %f <--\n", CL->result[i]);
	}
	printf("--> %f <--\n", CL->result[SIZE-4555]);
	clReleaseMemObject(CL->input);
    clReleaseMemObject(CL->output);
    clReleaseProgram(CL->program);
    clReleaseKernel(CL->kernel);
    clReleaseCommandQueue(CL->commands);
    clReleaseContext(CL->context);
}

void init_env(t_env *e)
{
	CL = malloc(sizeof(t_cl));
	//float final[SIZE];
	init_sources(e);
	init_opencl(e);

	getchar();
	/*float testa = 0.45;
	float testb = 55.224;
	float testc = 48.2555;
	float testd = 12.44;
	float teste = 65.12;
	float testf = 88.999;
	float testg = 48.11;
	float testh = 0.022;
	for (int t=0;t<480000;t++){
		if (t % 100 == 0){
			printf("%d\n", t);
		}
		for (int y = 0;y<50000;y++){

			   testa = 0.45;
			   testb = 55.224;
			   testc = 48.2555;
			   testd = 12.44;
			   teste = 65.12;
			   testf = 88.999;
			   testg = 48.11;
			   testh = 0.022;

			   testa *= testb;
			   testb /= testc;
			   testd /= testa;
			   teste *= testh;
			   testf /= testb;
			   testg *= testh;
			   testh /= testa;
	   }
	   final[t] = testa+testb+testc+testd+teste+testf+testg+testh;
	}*/

}
