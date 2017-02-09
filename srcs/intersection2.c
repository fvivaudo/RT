/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbompoil <mbompoil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:59:01 by fvivaudo          #+#    #+#             */
/*   Updated: 2016/12/16 12:15:58 by mbompoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

int		iraysphere(t_ray *r, t_obj *s, double *t0)
{
	double	abcdiscr[5];
	double	t[2];
	t_vec	dist;

	abcdiscr[0] = vectordot(r->dir, r->dir);
	dist = vectorsub(r->start, s->pos);
	abcdiscr[1] = 2 * vectordot(r->dir, dist);
	abcdiscr[2] = vectordot(dist, dist) - (s->rad * s->rad);
	abcdiscr[3] = abcdiscr[1] * abcdiscr[1] - 4 * abcdiscr[0] * abcdiscr[2];
	if (abcdiscr[3] < 0)
		return (FALSE);
	else
	{
		abcdiscr[4] = sqrtf(abcdiscr[3]);
		t[0] = (-(abcdiscr[1]) + abcdiscr[4]) / (2);
		t[1] = (-(abcdiscr[1]) - abcdiscr[4]) / (2);
		if (t[0] > t[1])
			t[0] = t[1];
		if ((t[0] > 0.001f) && (t[0] < *t0) && t[0] > 0)
		{
			*t0 = t[0];
			return (TRUE);
		}
	}
	return (FALSE);
}
