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
	(void)error;
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
