/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blinn_phong.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:34:48 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

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
