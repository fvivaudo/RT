

void	normalplane(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t)
{
	//vector dot > 0 check if both vectors are going in the same direction
	//if they are not, get the reverse direction of the plane to get it's normal
	if (vectordot(obj->dir, r->dir) >= 0.0)
	{
		comp->n = vectorscale(-1, obj->dir);
	}
	else
	{
		comp->n = obj->dir;
	}
	vectornormalize(&comp->n);
}

void	normalsphere(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t)
{
	//calcul de la normale d'une sphere
	//get distance between new distant ray and object pos?
	// distance = root of  (v1 - v2).(v1 - v2)

	double temp;
	t_vec inter_point;

	inter_point = vectoradd(r->start, vectorscale(*t, r->dir));
	comp->n = vectorsub(inter_point, obj->pos);
	temp = vectordot(comp->n, comp->n);
	if (temp == 0)
		return ;
	//the root of the dot product
	temp = 1.0f / sqrtf(temp);
	comp->n = vectorscale(temp, comp->n);
	vectornormalize(&comp->n);

	//if the intersection point is located behind the object center from the camera perspective,
	//then we're inside the object
	if (comp->reversen)
	{
		comp->n = vectorscale(-1, comp->n);
		comp->reversen = FALSE;
	}
}

void	normalcylinder(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t)
{
	t_vec	y_axis;
	t_vec	rot_axis;
	//t_vec	dir_cyl;
	t_vec	center_cyl;
	t_vec	inter_point;
	//t_vec	pos_cyl;
	inter_point = vectoradd(r->start, vectorscale(*t, r->dir));

	double	rot_angle;

	y_axis = vectorinit(0, 1, 0);
	rot_angle = acos(vectordot(obj->dir, y_axis));
	//get rotation angle

	rot_axis = vectorproduct(obj->dir, y_axis);
	//get orthogonal rotation vector

	vectornormalize(&rot_axis);

	if (rot_axis.x || rot_axis.y || rot_axis.z)
	{
		inter_point = vectorrotate(inter_point, rot_axis, rot_angle);
	}

	//get a point in the center of the cylinder at the same level as the intersection
	center_cyl = vectorinit(obj->pos.x, inter_point.y, obj->pos.z);
	//getting vector from center of cylinder slice toward the intersection point
	comp->n = vectorsub(inter_point, center_cyl);
	if (rot_axis.x || rot_axis.y || rot_axis.z)
	{
		comp->n = vectorrotate(comp->n, rot_axis, rot_angle);
	}
	vectornormalize(&comp->n);

	if (comp->reversen)
	{
		comp->n = vectorscale(-1, comp->n);
		comp->reversen = FALSE;
	}
}

//x = -r cos(rad)
//y = r
//z = -r sin(rad)
//n = inter_point - magnitude(newstart - cone.pos) / cos(e->alpha) * cone.dir
void	normalcone(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t)
{
	double tmp;
	t_vec inter_point;

	inter_point = vectoradd(r->start, vectorscale(*t, r->dir));

	//if the vector from the intersection to the apex is aligned to the cone axis, the it's fine
	if (vectordot(vectorsub(inter_point, obj->pos), obj->dir) > 0)
		tmp = vectormagnitude(vectorsub(inter_point, obj->pos)) / cos(obj->alpha);
	else
		tmp = vectormagnitude(vectorsub(inter_point, obj->pos)) / cos(obj->alpha) * (-1);

	//Removing object pos from the formula make it only true for a cone at the origin
	comp->n = vectorsub(inter_point, vectoradd(vectorscale(tmp, obj->dir), obj->pos));

	vectornormalize(&comp->n);

	if (comp->reversen)
	{
		comp->n = vectorscale(-1, comp->n);
		comp->reversen = FALSE;
	}
}

//xn = 2*A*xi + D*yi + E*zi + G
//yn = 2*B*yi + D*xi + F*zi + H
//z n = 2*C*zi + E*xi + F*yi + I
void	normalquadric(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t)
{
	t_vec 	camdir = vectorsub(obj->pos, r->start);
	double 	tmpdist = vectormagnitude(camdir);
	t_vec 	inter_point;

	inter_point = vectoradd(r->start, vectorscale(*t, r->dir));
	vectornormalize(&camdir);

//	t_vec tmp_start = vectorscale(-1, vectorscale(tmpdist, camdir));
	t_vec tmp_start = vectoradd(inter_point, vectorscale(-1, vectorscale(tmpdist + SCREEN_EYE_DIST, camdir)));

	comp->n.x = 2 * obj->quad.a * tmp_start.x;
	comp->n.x += 2 * obj->quad.f * tmp_start.y;
	comp->n.x += 2 * obj->quad.g * tmp_start.z;
	comp->n.x += 2 * obj->quad.p;
	comp->n.y = 2 * obj->quad.b * tmp_start.y;
	comp->n.y += 2 * obj->quad.f * tmp_start.x;
	comp->n.y += 2 * obj->quad.h * tmp_start.z;
	comp->n.y += 2 * obj->quad.q;
	comp->n.z = 2 * obj->quad.c * tmp_start.z;
	comp->n.z += 2 * obj->quad.g * tmp_start.x;
	comp->n.z += 2 * obj->quad.h * tmp_start.y;
	comp->n.z += 2 * obj->quad.r;
	vectornormalize(&comp->n);

	if (comp->reversen)
	{
		comp->n = vectorscale(-1, comp->n);
		comp->reversen = FALSE;
	}
	//if the vector from the intersection to the apex is aligned to the cone axis, the it's fine
}

	// P = inter_point
	// C = obj->pos
	// V = obj->dir
	// nrm = V (V/sqrt(V|V))

	// k = (P-C)|V
	// A = P - V*k
	// m = sqrt( r^2 - k^2 )
	// N = nrm( P - A - (C-A)*m/(R+m) )
void	normaltorus(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t)
{ 
	t_vec tmp[3];
	double m;
	double k;
	t_vec 	inter_point;
	
	inter_point = vectoradd(r->start, vectorscale(*t, r->dir));
	k = vectordot(vectorsub(inter_point, obj->pos), obj->dir);
	m = sqrtf((obj->rad2 * obj->rad2) - (k * k));
	tmp[0] = vectorsub(inter_point, vectorscale(k, obj->dir));
	tmp[1] = vectorscale(m, vectorsub(obj->pos, tmp[0]));
	tmp[2] = vectorsub(inter_point, tmp[0]);
	comp->n = vectorsub(tmp[2], (vectorscale(1.0 / (obj->rad + m), tmp[1])));

	vectornormalize(&comp->n);

	if (comp->reversen)
	{
		comp->n = vectorscale(-1, comp->n);
		comp->reversen = FALSE;
	}
}

void	normalget(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t)
{
		if (obj->type == TYPE_SPHERE)
			normalsphere(r, obj, comp, t);
		else if (obj->type == TYPE_PLANE)
			normalplane(r, obj, comp, t);
		else if (obj->type == TYPE_CYLINDER)
			normalcylinder(r, obj, comp, t);
		else if (obj->type == TYPE_CONE)
			normalcone(r, obj, comp, t);
		else if (obj->type == TYPE_QUADRIC)
			normalquadric(r, obj, comp, t);
		else if (obj->type == TYPE_TORUS)
			normaltorus(r, obj, comp, t);
}
