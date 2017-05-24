//rotate slice

//returns a distance
int		irayslice(__global t_ray *r, __global t_objgpu *obj, double *dist, t_objcomplement *comp)
{
	t_vec	tmp;
	double	tmpt;
	double	tmpdist;
	int		cursor;
	double	dot;

//	t_vec sliceinter;

	tmpdist = *dist;
	comp->reversen = FALSE;
	cursor = 1;
//	printf("slice->pos.x = %g, slice->pos.y = %g, slice->pos.z = %g\n", slice->pos.x, slice->pos.y, slice->pos.z);
	if (comp->t[0] < 0 && comp->t[1] < 0)
	{
		return (FALSE);
	}
/*	if (comp->t[0] > comp->t[1])
	{
		tmptswitch = comp->t[0];
		comp->t[0] = comp->t[1];
		comp->t[1] = tmptswitch;
	}*/

//relative dir should be a vector going from the slice toward emptiness/sliced part

	while (obj[cursor].type_obj == TYPE_SLICE)
	{
//case 0 =  unreachable slice, 2 cases, touching object or touching void
//case 1 =  intersection with plan, facing plan
//case 2 =  intersection with plan, not facing plan

//case 3 =  no intersection with plan and good dir
//case 3 =  no intersection with plan and wrong dir
//		printf("cursor->dir.x = %g cursor->dir.y = %g cursor->dir.z = %g\n", cursor->dir.x, cursor->dir.y, cursor->dir.z);
		tmp = vectorsub(vectoradd(obj[cursor].pos, obj->pos), r->start);
		dot = vectordot(obj[cursor].dir, r->dir);
		if (dot)
		{
			tmpt = vectordot(obj[cursor].dir, tmp) / dot;

//			sliceinter = vectoradd(r->start, vectorscale(tmpt, r->dir));
//			relativedir = vectorsub(relativepos, sliceinter); //from slice intersection toward sphere intersection
//	printf("relativedir.x = %g relativedir.y = %g relativedir.z = %g\n", relativedir.x, relativedir.y, relativedir.z);
//			vectornormalize(&relativedir);
//	printf("relativepos.x = %g relativepos.y = %g relativepos.z = %g\n", relativepos.x, relativepos.y, relativepos.z);
//	printf("relativedir.x = %g relativedir.y = %g relativedir.z = %g\n", relativedir.x, relativedir.y, relativedir.z);
			//printf("tmpt = %g\nt0 = %g\n comp->t[1] = %g, dot = %g\n", tmpt, comp->t[0], comp->t[1], dot);
			//check if slice is inside the object
			//if not, maybe we're hitting nothing, or we're hitting the object
			if ((comp->t[0] > tmpt && comp->t[1] > tmpt) || (comp->t[0] < tmpt && comp->t[1] < tmpt))
			{
				//comp->t[0] will be the closest point
				/*if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || (comp->t[0] < 0 && comp->t[1] > 0))
				{
					tmptswitch = comp->t[0];
					comp->t[0] = comp->t[1];
				}*/
			//	else
			//	{
				//	return (FALSE); //no conceivable distance, i think
			//	}


				//object is fully in front of slice and slice is aligned on r->dir
				if (comp->t[1] < tmpt && dot > 0)
				{
					//return(FALSE);
				//	printf("comp->t[0] = %g, comp->t[1] = %g, tmpt = %g\n", comp->t[0], comp->t[1], tmpt);
					if (comp->t[0] > 0 && comp->t[0] <= tmpdist)
					{
				//		printf("Outcome1 reached\n");
						tmpdist = comp->t[0];
					}
					else if (comp->t[1] > 0 && comp->t[1] <= tmpdist)
					{
				//		printf("Outcome2 reached\n");
						tmpdist = comp->t[1];
					}
				//	return (TRUE);
				}//object is fully behind slice and slice is directed toward r->start
				else if (comp->t[0] > tmpt && dot < 0)
				{
					if (comp->t[0] > 0 && comp->t[0] <= tmpdist)
					{
						tmpdist = comp->t[0];
					}
					else if (comp->t[1] > 0 && comp->t[1] <= tmpdist)
					{
						tmpdist = comp->t[1];
					}
				}//object is sliced out, resulting in an absence of collision, I guess
				else// if (comp->t[0] != tmpdist && comp->t[1] != tmpdist)
				{
				//	tmpdist = tmptswitch;
				//	return (TRUE);
					return (FALSE);
				}
				//	return (TRUE); //slice is out of object
			}
			else if (dot > 0 /*vectordot(relativedir, cursor->dir) > 0*/) // ray coming toward sliced surface
			{
				//comp->t[0] will be the farthest point

				//	return(FALSE);
				if (comp->t[1] > 0)
				{
					tmpdist = comp->t[1];
				}
				else if (comp->t[0] > 0)
				{
					tmpdist = comp->t[0];
				}
				//return (TRUE);
				//				tmpdist = tmpt;
				//comp->type = TYPE_PLANE;
			}
			else if (dot < 0 /*vectordot(relativedir, cursor->dir) < 0*/) // ray coming toward object untouched surface
			{
				//comp->t[0] will be the closest point
				comp->reversen = TRUE; // not logic, but it works, need to check if it works everywhere..
				if (comp->t[0] > 0)
				{
					tmpdist = comp->t[0];
				}
				else if (comp->t[1] > 0)
				{
					tmpdist = comp->t[1];
				}
				//return (TRUE);
				//comp->type = TYPE_PLANE;
			}
		}
		++cursor;
	}
	*dist = tmpdist;
	normalget(r, obj, comp, dist);
	if (comp->reversen)
	{
		comp->reversen = FALSE;
		vectorscale(-1, comp->n);
	}
				//	*dist = t0;
	return (TRUE);
}

//The problem lies in computeshadow, do my intersection works well when i go from the hole toward light?
int		irayneg(__global t_ray *r, __global t_objgpu *obj, double *dist, t_objcomplement *comp)
{
	int				cursor;
	t_objcomplement	deepestobj;
	double 			tmax;
	double  		holet[2];
	t_objcomplement compbis;
//	t_vec sliceinter;

	holet[0] = 0;
	holet[1] = 0;
	cursor = 1;
	//comp->normobj = NULL;
	compbis.reversen = TRUE;
	compbis.n = vectorinit(0, 0, 0);
//	printf("slice->pos.x = %g, slice->pos.y = %g, slice->pos.z = %g\n", slice->pos.x, slice->pos.y, slice->pos.z);
	if (comp->t[0] < 0 && comp->t[1] < 0)
	{
		return (FALSE);
	}

/*		printf("\n");

	deepestobj = cursor;
	while (deepestobj)
	{
		printf("type = %d, compbis.t[0] = %g, compbis.t[1] = %g, deepestobj->rad = %g, deepestobj->pos.x = %g, deepestobj->pos.y = %g, deepestobj->pos.z = %g, deepestobj->dir.x = %g, deepestobj->dir.y = %g, deepestobj->dir.z = %g\n", compbis.type, compbis.t[0], compbis.t[1], deepestobj->rad, deepestobj->pos.x, deepestobj->pos.y, deepestobj->pos.z, deepestobj->dir.x, deepestobj->dir.y, deepestobj->dir.z);
		deepestobj = deepestobj->nextitem;
	}*/

//printf("\n");
	while (obj[cursor].type_obj == TYPE_NEGATIVE)
	{
		//printf("type = %d, compbis.t[0] = %g, compbis.t[1] = %g, holet[0] = %g, holet[1] = %g\n", cursor->type, compbis.t[0], compbis.t[1], holet[0], holet[1]);
		tmax = -1;
		if ((obj[cursor].type == TYPE_SPHERE && iraysphere(r, &obj[cursor], &tmax, &compbis)) ||
			(obj[cursor].type == TYPE_PLANE && irayplane(r, &obj[cursor], &tmax, &compbis)) ||
			(obj[cursor].type == TYPE_CYLINDER && iraycylinder(r, &obj[cursor], &tmax, &compbis)) ||
			(obj[cursor].type == TYPE_CONE && iraycone(r, &obj[cursor], &tmax, &compbis)) ||
			(obj[cursor].type == TYPE_TORUS && iraytorus(r, &obj[cursor], &tmax, &compbis)) ||
			(obj[cursor].type == TYPE_QUADRIC && irayquadric(r, &obj[cursor], &tmax, &compbis)))
		//	(obj[cursor].type == TYPE_TORUS && iraytorus(r, cursor, &tmax, &compbis)))
		{
			if (compbis.t[1] == DOESNOTEXIST)
			{
				continue;
			}
			if (compbis.t[0] > compbis.t[1]) //doesnt matter if holet[0] is inferior to 0 here
			{
				swapdouble(&compbis.t[0], &compbis.t[1]);
			}
			//first negative object
			if (holet[0] == 0 && holet[1] == 0)
			{
				deepestobj.n = compbis.n;
			//	comp->normobj = cursor;
			//	comp->normal = cursor->normal;
				holet[0] = compbis.t[0];
				holet[1] = compbis.t[1];
			//	if (holet[0] > holet[1]) //doesnt matter if holet[0] is inferior to 0 here
			//	{
			//		swapdouble(&holet[0], &holet[1]);
			//	}
			}
			else if (holet[0] <= compbis.t[1] && compbis.t[0] <= holet[1]) // check for overlap
			{
				if (compbis.t[0] < holet[0])
				{
					holet[0] = compbis.t[0];
				}
				if (compbis.t[1] > holet[1])
				{
					deepestobj.n = compbis.n;
				//	comp->normobj = cursor;
				//	comp->normal = cursor->normal;
					holet[1] = compbis.t[1];
				}
			}
		}
		//printf("type = %d, compbis.t[0] = %g, compbis.t[1] = %g, holet[0] = %g, holet[1] = %g\n", cursor->type, compbis.t[0], compbis.t[1], holet[0], holet[1]);
		//it should be nextitem and not nextneg
		++cursor;
	}
		//case 1 hole going through object
		//case 2 hole in front object
		//case 3 hole behind object
//	if (e->x == 355 && e->y == 263)
//	{
//		printf("case 0 = holet[0] = %g, holet[1] = %g, comp->t[0] = %g, comp->t[1] = %g\n", holet[0], holet[1], comp->t[0], comp->t[1]);
//	}
//if we are inside the object how do we determine how to
	if (holet[0] < comp->t[0] && holet[1] > comp->t[1])
	{
//	if (e->x == 355 && e->y == 263)
//		printf("case 1 = holet[0] = %g, holet[1] = %g, comp->t[0] = %g, comp->t[1] = %g\n", holet[0], holet[1], comp->t[0], comp->t[1]);
		return (FALSE);
	}
	else if (holet[0] < comp->t[0] && holet[1] > comp->t[0])
	{
			comp->n = vectorscale(-1, deepestobj.n);
			*dist = holet[1]; //we need the deepest hole
	}
	else
	{
		*dist = comp->t[0];
		normalget(r, obj, comp, dist);
	}				//	*dist = t0;
	return (TRUE);
}


int		irayplane(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp)
{
	t_vec	tmp;
	double	t[2];
	bool	ret;
	int 	cursor;
	//printf("plane\n");
	ret = FALSE;
	tmp = vectorsub(obj->pos, r->start);
	if (vectordot(obj->dir, r->dir) != 0)
	{
		t[0] = vectordot(obj->dir, tmp) / vectordot(obj->dir, r->dir);
	}
	else
	{
		return (FALSE);
	}

	comp->t[0] = t[0];
	comp->t[1] = DOESNOTEXIST;

	if (t[0] < 0)
	{
		return (FALSE);
	}

	//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
	cursor = 1;
	if (obj[1].type_obj == TYPE_SLICE)
	{
		ret = irayslice(r, obj, t0, comp);
		if (!ret)
		{
			return (FALSE);
		}
		while (obj[++cursor].type_obj == TYPE_SLICE);
	}
	if (obj[cursor].type_obj == TYPE_NEGATIVE)
	{
		ret = irayneg(r, obj, t0, comp);
		if (!ret)
		{
			return (FALSE);
		}
	}
	if (ret)
	{
		return (TRUE);
	}
	if (/*(t[0] > 0.001f) && */t[0] < *t0 || *t0 == -1)
	{
		*t0 = t[0];
		normalplane(r, obj, comp, t0);
		return (TRUE);
	}
	return (FALSE);
}

int		iraysphere(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp)
{
	double	abcdiscr[5];
	t_vec	dist;
	bool	ret;
	int 	cursor;

	ret = FALSE;
	//double tmpt;

	abcdiscr[0] = vectordot(r->dir, r->dir);
	dist = vectorsub(r->start, obj->pos);
	abcdiscr[1] = 2 * vectordot(r->dir, dist);
	abcdiscr[2] = vectordot(dist, dist) - (obj->rad * obj->rad);
	abcdiscr[3] = abcdiscr[1] * abcdiscr[1] - 4 * abcdiscr[0] * abcdiscr[2];

	if (abcdiscr[3] < 0)
	{
		return (FALSE);
	}
	else
	{
		abcdiscr[4] = sqrtf(abcdiscr[3]);
		comp->t[0] = (-(abcdiscr[1]) + abcdiscr[4]) / (2);
		comp->t[1] = (-(abcdiscr[1]) - abcdiscr[4]) / (2);

		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0) // this is not necesarily true, if i have a negative object, it can extend T and bring it into view
		{
			return(FALSE);
		}
/*		else if (t[0] < 0)
		{
			t[0] = t[1];
		}*/

		cursor = 1;
		if (obj[1].type_obj == TYPE_SLICE)
		{
			ret = irayslice(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
			while (obj[++cursor].type_obj == TYPE_SLICE);
		}
		if (obj[cursor].type_obj == TYPE_NEGATIVE)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}

		if (/*(t[0] > 0.001f) && */(comp->t[0] < *t0) || *t0 == -1)
		{
		//printf("tfinal = %g\n", t[0]);

			*t0 = comp->t[0];
			normalsphere(r, obj, comp, t0);
			return (TRUE);
		}
	}
	return (FALSE);
}


int		iraycone(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp)
{
	t_vec	delt_p;
	t_vec	tmp[3];
	double	abcd[4];
	bool	ret;
	int 	cursor;

	ret = FALSE;
	delt_p = vectorinit(r->start.x - obj->pos.x, r->start.y - obj->pos.y,
	r->start.z - obj->pos.z);
	tmp[0] = vectorscale(vectordot(r->dir, obj->dir), obj->dir);
	tmp[1] = vectorsub(r->dir, tmp[0]);
	tmp[0] = vectorscale(vectordot(delt_p, obj->dir), obj->dir);
	tmp[2] = vectorsub(delt_p, tmp[0]);
	abcd[0] = (pow(cos(obj->alpha), 2)
	* vectordot(tmp[1], tmp[1])) - (pow(sin(obj->alpha), 2)
	* pow(vectordot(r->dir, obj->dir), 2));
	abcd[1] = 2 * ((pow(cos(obj->alpha), 2)
	* vectordot(tmp[1], tmp[2])) - (pow(sin(obj->alpha), 2)
	* vectordot(r->dir, obj->dir) * vectordot(delt_p, obj->dir)));
	abcd[2] = (pow(cos(obj->alpha), 2)
	* vectordot(tmp[2], tmp[2])) - (pow(sin(obj->alpha), 2)
	* pow(vectordot(delt_p, obj->dir), 2));
	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
	if (abcd[3] >= 0)
	{
		comp->t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		comp->t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);

		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0)
		{
			return(FALSE);
		}

		//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
		cursor = 1;
		if (obj[1].type_obj == TYPE_SLICE)
		{
			ret = irayslice(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
			while (obj[++cursor].type_obj == TYPE_SLICE);
		}
		if (obj[cursor].type_obj == TYPE_NEGATIVE)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (/*(t[0] > 0.001f) && */comp->t[0] < *t0 || *t0 == -1)
		{
			*t0 = comp->t[0];
			normalcone(r, obj, comp, t0);
			return (TRUE);
		}
	}
	return (FALSE);
}


//Aq = Axd2 + Byd2 + Czd2 + Dxdyd + Exdzd + Fydzd
//Bq = 2*Axoxd + 2*Byoyd + 2*Czozd + D(xoyd + yoxd) + E(xozd + zoxd) + F(yozd + ydzo) + Gxd + Hyd + Izd
//Cq = Axo2 + Byo2 + Czo2 + Dxoyo + Exozo + Fyozo + Gxo + Hyo + Izo + J
int		irayquadric(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp)
{
	double	abcd[4];
	bool	ret;
	t_vec 	cam;
	int 	cursor;

	//r->dir is wrong if there are reflections


	ret = FALSE;
	t_vec camdir = vectorsub(obj->pos, r->initialstart);
	double tmpdist = vectormagnitude(camdir);
	vectornormalize(&camdir);
	t_vec tmp_start = vectoradd(r->start, vectorscale(-1, vectorscale(tmpdist + SCREEN_EYE_DIST, camdir)));
	// the object is always at the origin, we simulate a distance between the eye and the object
	//to change it's position
	//	t_vec tmp_start = vectorinit(0, 0, 0);
	//	printf("tmp_start.x = %g\n"
	//		"tmp_start.y = %g\n"
	//		"tmp_start.z = %g\n", tmp_start.x, tmp_start.y, tmp_start.z);

	cam = r->dir;
	abcd[0] = obj->quad.a * pow(r->dir.x, 2);
	abcd[0] += obj->quad.b * pow(r->dir.y, 2);
	abcd[0] += obj->quad.c * pow(r->dir.z, 2);
	abcd[0] += obj->quad.f * r->dir.x * r->dir.y;
	abcd[0] += obj->quad.g * r->dir.x * r->dir.z;
	abcd[0] += obj->quad.h * r->dir.y * r->dir.z;

	abcd[1] = 2 * obj->quad.a * tmp_start.x * r->dir.x;
	abcd[1] += 2 * obj->quad.b * tmp_start.y * r->dir.y;
	abcd[1] += 2 * obj->quad.c * tmp_start.z * r->dir.z;
	abcd[1] += obj->quad.f * (tmp_start.x * r->dir.y + tmp_start.y * r->dir.x);
	abcd[1] += obj->quad.g * (tmp_start.x * r->dir.z + tmp_start.z * r->dir.x);
	abcd[1] += obj->quad.h * (tmp_start.y * r->dir.z + tmp_start.z * r->dir.y);
	abcd[1] += obj->quad.p * r->dir.x;
	abcd[1] += obj->quad.q * r->dir.y;
	abcd[1] += obj->quad.r * r->dir.z;

	abcd[2] = obj->quad.a * pow(tmp_start.x, 2);
	abcd[2] += obj->quad.b * pow(tmp_start.y, 2);
	abcd[2] += obj->quad.c * pow(tmp_start.z, 2);
	abcd[2] += obj->quad.f * tmp_start.x * tmp_start.y;
	abcd[2] += obj->quad.g * tmp_start.x * tmp_start.z;
	abcd[2] += obj->quad.h * tmp_start.y * tmp_start.z;
	abcd[2] += obj->quad.p * tmp_start.x;
	abcd[2] += obj->quad.q * tmp_start.y;
	abcd[2] += obj->quad.r * tmp_start.z;
	abcd[2] += -obj->quad.d;

	if (abcd[0] == 0) // solving first degree equation
	{
		comp->t[0] = -abcd[2] / abcd[1];
		if (comp->t[0] > 0 && comp->t[0] < *t0)
		{
			*t0 = comp->t[0];
			return (TRUE);
		}
		return (FALSE);
	}
	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);
	if (abcd[3] >= 0)
	{
		comp->t[0] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
	//	if (comp->t[0] <= 0)
			comp->t[1] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
	//	else
	//		comp->t[1] = MAX_RANGE + 1;
		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0)
		{
			return(FALSE);
		}
		cursor = 1;
		if (obj[1].type_obj == TYPE_SLICE)
		{
			ret = irayslice(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
			while (obj[++cursor].type_obj == TYPE_SLICE);
		}
		if (obj[cursor].type_obj == TYPE_NEGATIVE)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (/*(t[0] > 0.001f) && */comp->t[0] < *t0 || *t0 == -1)
		{
			*t0 = comp->t[0];
			normalquadric(r, obj, comp, t0);
			return (TRUE);
		}
	}
	return (FALSE);
}

int		iraycylinder(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp)
{
	double	abcd[4];
	t_vec	cam;
	bool	ret;
	int 	cursor;

	ret = FALSE;
	cam = r->dir;
	t_vec origin = vectorsub(r->start, obj->pos);
	t_vec tmp1 = vectorsub(r->dir, vectorscale(vectordot(r->dir, obj->dir), obj->dir));
	t_vec tmp2 = vectorsub(origin, vectorscale(vectordot(origin, obj->dir), obj->dir));

	abcd[0] = vectordot(tmp1, tmp1);
	abcd[1] = 2 * vectordot(tmp1, tmp2);
	abcd[2] = vectordot(tmp2, tmp2) - pow(obj->rad, 2);
	abcd[3] = pow(abcd[1], 2) - (4 * abcd[0] * abcd[2]);

	if (abcd[3] < 0)
		return (FALSE);
	else
	{
		comp->t[0] = (((-1) * abcd[1]) + sqrtf(abcd[3])) / (2 * abcd[0]);
		comp->t[1] = (((-1) * abcd[1]) - sqrtf(abcd[3])) / (2 * abcd[0]);
		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0)
		{
			return(FALSE);
		}
		cursor = 1;
		if (obj[1].type_obj == TYPE_SLICE)
		{
			ret = irayslice(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
			while (obj[++cursor].type_obj == TYPE_SLICE);
		}
		if (obj[cursor].type_obj == TYPE_NEGATIVE)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}
		if (comp->t[0] < *t0 || *t0 == -1) // -1 only useful for neg
		{
			*t0 = comp->t[0];
			normalcylinder(r, obj, comp, t0);
			return (TRUE);
		}
	}
	return (FALSE);
}

//negative object model working with shadow
//a negative object going through an object (t0 and t1 of negative larger than both t0 and t1 of native object)
int		iraytorus(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp)
{
	bool	ret;
	double	abcdiscr[5];
//	double	tmp[5];
//	t_vec	dist;
	double	t[4];
	int 	nb_root;
	int 	cursor;

	t_vec rayOriginPosition = r->start;
	t_vec rayDirection = r->dir;

	t_vec centerToRayOrigin = vectorsub(rayOriginPosition, obj->pos);
	const double centerToRayOriginDotDirection = vectordot(rayDirection, centerToRayOrigin);
	double	centerToRayOriginDotDirectionSquared = vectordot(centerToRayOrigin, centerToRayOrigin);
	double innerRadiusSquared = obj->rad2 * obj->rad2;
	double outerRadiusSquared = obj->rad * obj->rad;

	double	axisDotCenterToRayOrigin	= vectordot(obj->dir, centerToRayOrigin);
	double	axisDotRayDirection = vectordot(obj->dir, rayDirection);
	double	a = 1 - axisDotRayDirection * axisDotRayDirection;
	double	b = 2 * (vectordot(centerToRayOrigin, rayDirection) - axisDotCenterToRayOrigin * axisDotRayDirection);
	double  c = centerToRayOriginDotDirectionSquared - axisDotCenterToRayOrigin * axisDotCenterToRayOrigin;
	double	d = centerToRayOriginDotDirectionSquared + outerRadiusSquared - innerRadiusSquared;

	// Solve quartic equation with coefficients A, B, C, D and E
	abcdiscr[0] = 1;
	abcdiscr[1] = 4 * centerToRayOriginDotDirection;
	abcdiscr[2] = 2 * d + abcdiscr[1] * abcdiscr[1] * 0.25f - 4 * outerRadiusSquared * a;
	abcdiscr[3] = abcdiscr[1] * d - 4 * outerRadiusSquared * b;
	abcdiscr[4] = d * d - 4 * outerRadiusSquared * c;
//	abcdiscr[1] /= abcdiscr[0];
//	abcdiscr[2] /= abcdiscr[0];
//	abcdiscr[3] /= abcdiscr[0];
//	abcdiscr[4] /= abcdiscr[0];

	nb_root = SolveP4(t, abcdiscr[1], abcdiscr[2], abcdiscr[3], abcdiscr[4]);

	if (nb_root)
	{
		comp->t[0] = t[0];
		comp->t[1] = t[1];

		if ((comp->t[0] > comp->t[1] && comp->t[1] > 0) || comp->t[0] < 0)
		{
			swapdouble(&comp->t[0], &comp->t[1]);
		}

		if (comp->t[0] < 0 && comp->t[1] < 0) // this is not necesarily true, if i have a negative object, it can extend T and bring it into view
		{
			return(FALSE);
		}


		//printf("t[0] = %g\nt[1] = %g\n", t[0], t[1]);
		cursor = 1;
		if (obj[1].type_obj == TYPE_SLICE)
		{
			ret = irayslice(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
			while (obj[++cursor].type_obj == TYPE_SLICE);
		}
		if (obj[cursor].type_obj == TYPE_NEGATIVE)
		{
			ret = irayneg(r, obj, t0, comp);
			if (!ret)
			{
				return (FALSE);
			}
		}
		if (ret)
		{
			return (TRUE);
		}

		if ((comp->t[0] < *t0) || *t0 == -1)
		{
		//printf("tfinal = %g\n", t[0]);

			*t0 = comp->t[0];
			normaltorus(r, obj, comp, t0);
			return (TRUE);
		}
	}
	return (FALSE);
}

__kernel void actionn(__global t_ray *input, __global t_gpu_out *output, const unsigned int count, __global t_objgpu *objs, __global t_lightgpu *lights)
{
	t_color_data data;
	/*
		x = i % HEIGHT
		y = i / HEIGHT
	*/
	//simple object, then list of negatives, then slices
	 unsigned int i;
	 int j;

	 i = get_global_id(0);
	 j = 0;
	 if (i <= count)
	 {
		 //int x = i % 600;
		 //int y = i / 600;
		 double t = MAX_RANGE;
		 double mint = 9999999;
		 int finalid = -1;
		 t_vec finalnormal = vectorinit(0, 0, 0);

		//for (int i = 0; objs[i].type != 0; ++i)
		//	printf("objs[i].type = %d\n", objs[i].type);
		 while (objs[j].type_obj != 0)
		 {
		 	t_objcomplement comp;


		//	printf("j = %d\n", j);
		//	printf("objs[j].type_obj = %d\n", objs[j].type_obj);
		 	if (objs[j].type_obj == TYPE_OBJECT)
		 	{
		 	//printf("ok\n");
		//		if ((objs[j].type == TYPE_SPHERE && iraysphere(&input[i], &objs[j], &t, &comp)) ||
		//			(objs[j].type == TYPE_PLANE && irayplane(&input[i], &objs[j], &t, &comp)))
				 if ((objs[j].type == TYPE_SPHERE && iraysphere(&input[i], &objs[j], &t, &comp)) ||
			 	(objs[j].type == TYPE_PLANE && irayplane(&input[i], &objs[j], &t, &comp)) ||
				(objs[j].type == TYPE_QUADRIC && irayquadric(&input[i], &objs[j], &t, &comp)) ||
				(objs[j].type == TYPE_CONE && iraycone(&input[i], &objs[j], &t, &comp)) ||
				(objs[j].type == TYPE_TORUS && iraytorus(&input[i], &objs[j], &t, &comp)) ||
				(objs[j].type == TYPE_CYLINDER && iraycylinder(&input[i], &objs[j], &t, &comp)))
				{
					if (t < mint)
					{
						finalnormal = comp.n;
						mint = t;
						finalid = j;
						data.n = comp.n;
					}
			//		printf("comp.n.x = %g, comp.n.y = %g, comp.n.z = %g\n", comp.n.x, comp.n.y, comp.n.z);
				}
			}
			++j;
		}
	//	printf("j = %d\n", j);
	//	printf("count = %d\n", count);
		data.ray_dir = input[i].dir;
		data.inter_point = vectoradd(input[i].start, vectorscale(t, input[i].dir));
		data.cmat = objs[finalid].material;
		data.coef = 1;

		if ((data.cmat.transparency + data.cmat.reflection) < 1.0)
			output[i].color = deal_shadow(&output[i].r, objs, lights, &data); // cant modify input so soon, need it in cpu
		else
			output[i].color = colorinit(0, 0, 0); // cant modify input so soon, need it in cpu

		output[i].id = finalid;
		output[i].r = input[i];
		output[i].t = t;
		output[i].normal = data.n;
	}
}
