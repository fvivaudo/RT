
t_color		colorinit(double red, double green, double blue)
{
	t_color	col;

	col.red = red;
	col.green = green;
	col.blue = blue;
	return (col);
}

//I consider an object like a series of wall instead of a single entity, it might not be the best way to calculate
// transparency, refraction
double		computeshadow(__global t_ray *r, __global t_objgpu *obj, double light, double dist)
{
	t_objcomplement	comp;
	int i;

	comp.reversen = FALSE;
	i = 0;
//	double t = dist; // distance between point and light
	//printf("type == %d\n", cursor->type);
	//if (cursor->type == TYPE_SPHERE)
	//	printf("Transparency computeshadow == %g\n",cursor->material.transparency);

	 	while (obj[i].type_obj == TYPE_OBJECT)
	 	{
	 	//printf("ok\n");
	//		if ((obj[i].type == TYPE_SPHERE && iraysphere(&input[i], &obj[i], &t, &comp)) ||
	//			(obj[i].type == TYPE_PLANE && irayplane(&input[i], &obj[i], &t, &comp)))
			 if ((obj[i].type == TYPE_SPHERE && iraysphere(r, &obj[i], &dist, &comp)) ||
		 	(obj[i].type == TYPE_PLANE && irayplane(r, &obj[i], &dist, &comp)) ||
			(obj[i].type == TYPE_QUADRIC && irayquadric(r, &obj[i], &dist, &comp)) ||
			(obj[i].type == TYPE_CONE && iraycone(r, &obj[i], &dist, &comp)) ||
			(obj[i].type == TYPE_TORUS && iraytorus(r, &obj[i], &dist, &comp)) ||
			(obj[i].type == TYPE_CYLINDER && iraycylinder(r, &obj[i], &dist, &comp)))
			{
			//	e->t = t;
				//printf("e->cmat.transparency = %g\n", e->cmat.transparency);
				light *= obj[i].material.transparency; // is it accurate?
			}
			++i;
		}
	//if (cursor->type == TYPE_SPHERE)
	//	printf("Transparency computeshadow 1 == %g\n", e->cmat.transparency);
	//	printf("Transparency computeshadow 1 == %g\n", e->cmat.transparency);
	//	ft_putendl("Transparency computeshadow 1 == %g\n",cursor->material.transparency);
	return (light);
	// if an intersection was found
}

void	blinn_phong(t_color *col, t_vec lightray_dir, t_color_data *data)
{
	double			tmp;
	double			blinnterm;
	t_vec			blinnDir;

	blinnDir = vectorsub(lightray_dir, data->ray_dir);
	tmp = sqrtf(vectordot(blinnDir, blinnDir));
	if (tmp)
	{
		blinnDir = vectorscale((1 / tmp), blinnDir);
		tmp = vectordot(blinnDir, data->n);
		blinnterm = tmp ? tmp : 0;
		blinnterm = data->cmat.brillance * pow(blinnterm, SPEC_POW)/* * data->coef*/;
		(*col).red += data->cmat.brillance * blinnterm;
		(*col).green += data->cmat.brillance * blinnterm;
		(*col).blue += data->cmat.brillance * blinnterm;
	}
}

void	perlinmap (t_vec inter_point, t_mat *cmat)
{
	t_vec output;
	double noiseCoef = 0;

	for (int level = 1; level < 10; ++level)
	{
		noiseCoef += (1.0 / level) * fabs(noise(level * (cmat->procedural_scale) * inter_point.x, level * (cmat->procedural_scale) * inter_point.y, level * (cmat->procedural_scale) * inter_point.z));
	};
	//noiseCoef = 0.5 * sin((inter_point.x + inter_point.y) * e->cmat.procedural_scale + noiseCoef) + 0.5;
	output = vectoradd(
	vectorscale(noiseCoef, *(t_vec*)&cmat->diffuse)
	,vectorscale((1.0 - noiseCoef), *(t_vec*)&cmat->diffuse2));


	cmat->diffuse.red = output.x;

	cmat->diffuse.green = output.y;
	cmat->diffuse.blue = output.z;
}

t_vec	bump_mapping(t_vec inter_point, t_mat *cmat, t_vec n)
{
	t_vec	tmp;
	t_vec	new_n;
	double	temp;

	tmp = vectorscale(0.1, inter_point);
	double noiseCoefx = noise(tmp.x, tmp.y, tmp.z);
	double noiseCoefy = noise(tmp.y, tmp.z, tmp.x);
	double noiseCoefz = noise(tmp.z, tmp.x, tmp.y);
	new_n.x = (1.0f -  cmat->bump ) * n.x +  cmat->bump * noiseCoefx;
	new_n.y = (1.0f -  cmat->bump ) * n.y +  cmat->bump * noiseCoefy;
	new_n.z = (1.0f -  cmat->bump ) * n.z +  cmat->bump * noiseCoefz;
	temp = vectordot(new_n ,new_n);
	if (temp == 0.0f)
		return (n);
	temp = 1.0 / sqrtf(temp);
	new_n = vectorscale(temp, new_n);
	vectornormalize(&new_n);
	return (new_n);
}

void	marble (t_vec inter_point, t_mat *cmat)
{
	t_vec output;
	double noiseCoef = 0;

	for (int level = 1; level < 10; level++)
	{
		noiseCoef += (1.0 / level) * fabs(noise(level * cmat->procedural_scale * inter_point.x, level * cmat->procedural_scale * inter_point.y, level * cmat->procedural_scale * inter_point.z));
	};
	noiseCoef = 0.5 * sin((inter_point.x + inter_point.y) * cmat->procedural_scale + noiseCoef) + 0.5;
	output = vectoradd(
	vectorscale(noiseCoef, *(t_vec*)&cmat->diffuse)
	,vectorscale((1.0 - noiseCoef), *(t_vec*)&cmat->diffuse2));
	cmat->diffuse.red = output.x;
	cmat->diffuse.green = output.y;
	cmat->diffuse.blue = output.z;
}


void	checker (t_vec inter_point, t_mat *cmat)
{
	if (((int)floor(inter_point.x / (1.0 / cmat->procedural_scale))
		+ (int)floor(inter_point.y / (1.0 / cmat->procedural_scale))
		+ (int)floor(inter_point.z / (1.0 / cmat->procedural_scale))) % 2 == 0)
	{
		cmat->diffuse = cmat->diffuse2;
	}
}

/*for each light in the scene
**{
**	if the light is not in shadow of another object
**	{
**		add this light contribution to computed color;
**	}
**}*/
t_color		deal_shadow(__global t_ray *lightray, __global t_objgpu *obj, __global t_lightgpu *lights, t_color_data *data)
{

	double			tmpdot;
	double			distancetolight;
	int 			i;
	t_color 		col;
	t_vec 			lightpointdist;
	double 			lambert;
//	t_obj			*cursor;

	col = colorinit(0, 0, 0);
//	cursor = e->obj;

	i = 0;
	while (lights[i].set)
	{
		if (data->cmat.bump)
		{
			data->n = bump_mapping(data->inter_point, &data->cmat, data->n);
		}
		//if (cursor->type == TYPE_SPHERE)
		//	printf("Transparency computeshadow 2 == %g\n",cursor->material.transparency);
		++i;
		//distance between intersection point and light
		lightpointdist = vectorsub(lights[i - 1].pos, data->inter_point);
		distancetolight = vectormagnitude(lightpointdist);
//		printf("Checkpoint0\n");
/*		printf("data->inter_point.x = %g, data->inter_point.y = %g, data->inter_point.z = %g\n", data->inter_point.x, data->inter_point.y, data->inter_point.z);
		printf("e->clight.pos.x = %g, e->clight.pos.y = %g, e->clight.pos.z = %g\n", e->clight.pos.x, e->clight.pos.y, e->clight.pos.z);
		printf("n.x = %g, n.y = %g, n.z = %g\n", n.x, n.y, n.z);
		printf("lightpointdist.x = %g, lightpointdist.y = %g, lightpointdist.z = %g\n", lightpointdist.x, lightpointdist.y, lightpointdist.z);
		printf("e->t = %g\n", e->t);*/


		//	printf("Transparency == %g\n",e->transcoef);

		//if the vector dot is negative, then both vectors are going in opposite directions
		vectornormalize(&lightpointdist);
		//printf("type == %d\n", cursor->type);
		//if (e->transcoef == 0.0)
	//	{
			if ((tmpdot = vectordot(data->n, lightpointdist)) <= 0.0f)
				continue;
	//	}
	//	while (cursor)
		//{/
		//	if (cursor->type == TYPE_SPHERE)
		//		printf("Transparency computeshadow 1 == %g\n",cursor->material.transparency);
		//	cursor = cursor->nextitem;
		//}
/*		if (e->transcoef == 0.0)
		{
			e->t = vectormagnitude(lightpointdist);
			if (e->t <= 0.0f) // what's the point of changing e->t now?
				continue; // delete later, eventually
		}*/
		//Check if there is something between point and light
		//limits light range, or does it?
	//	lightray.dir = vectorscale((1 / e->t), lightpointdist); //alternative to normalization?
		lightray->dir = lightpointdist;
		lightray->start = vectoradd(data->inter_point, lightpointdist);
		//lightray.start = vectoradd(data->inter_point, lightray.dir);
		// the object risks a colision with itself, which is quite problematic
		// 2 solutions :
		// -forbid colision detection with itself
		// -launch the ray a small distance away from the actual intersection point (CURRENT)
		//printf("intersection\n");
	//	printf("intersection\n");
/*		if (intersection(e, &lightray, -1))
		{
			continue;
		}*/

		double light = 1;
		//temporary removal
		if ((light = computeshadow(lightray, obj, light, distancetolight)) == 0.0) // 1 is light, is it ok?
		{
			continue;
		}

		if (data->cmat.type == TEXTURE_MARBLE)
			marble(data->inter_point, &data->cmat);
		else if (data->cmat.type == TEXTURE_CHECKER)
			checker(data->inter_point, &data->cmat);
		else if (data->cmat.type == TEXTURE_PERLIN)
			perlinmap(data->inter_point, &data->cmat);


	//	printf("light = %g\n", light);
	//	printf("intersection2\n");

	//	lambert diffusion
		//if (cursor->material.transparency == 0)
	//	if (e->transcoef == 0.0)
		lambert = tmpdot * data->coef;
	//	else
	//		e->lambert = 1;
		col.red += lambert * lights[i - 1].intensity.red * data->cmat.diffuse.red * light;
		col.green += lambert * lights[i - 1].intensity.green * data->cmat.diffuse.green * light;
		col.blue += lambert * lights[i - 1].intensity.blue * data->cmat.diffuse.blue * light;

		//check collision with objects between object and light

		// Blinn Phong model
		blinn_phong(&col, lightray->dir, data);
	}
	return (col);
	//Ambient lighting

	//	printf("e->red = %g, e->green = %g, e->blue = %g\n", e->red, e->green, e->blue);
}
