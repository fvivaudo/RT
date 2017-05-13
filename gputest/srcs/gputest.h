/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projet.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/11 19:59:46 by momartin          #+#    #+#             */
/*   Updated: 2017/01/11 19:59:48 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPUTEST_H
# define GPUTEST_H

#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "./libft/libft.h"
#include <OpenCL/opencl.h>

# define CL e->cl
# define SIZE 4800

typedef struct 	s_cl{
	char 			*source;
	cl_device_id 	device_id;
	cl_context 		context;
	cl_command_queue commands;
	cl_program 		program;
	cl_kernel 		kernel;
	int 			err;
	cl_mem 			input;
	cl_mem 			output;
	size_t 			local;
	size_t 			global;
	float 			data[SIZE];
	float 			result[SIZE];
	unsigned int 	count;
}				t_cl;

typedef struct	s_env
{
	t_cl 		*cl;
}				t_env;

void	init_env(t_env *e);

#endif
