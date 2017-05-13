/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_mapping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:36:00 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

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
