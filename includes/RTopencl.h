
# define PROCEDURAL_SCALE 		0.05
# define TWOPI  				6.28318530717958648

# define TRUE 					1
# define FALSE 					0

# define WIDTH					800
# define HEIGHT					600
# define KEPSILON				0.0001
# define FOV					120
# define MAX_RANGE				20000.0
# define MAX_DEPTH_LEVEL		5
# define AMBIANT_SHADING		0.15
# define SPEC_POW				20.0
# define EXPOSURE				-1

# define TEXTURE_MARBLE			1
# define TEXTURE_CHECKER		2
# define TEXTURE_PERLIN		 	3

# define TYPE_OBJECT			1
# define TYPE_SLICE				2
# define TYPE_NEGATIVE			3

# define TYPE_SPHERE			1
# define TYPE_PLANE				2
# define TYPE_CYLINDER			3
# define TYPE_CONE				4
# define TYPE_QUADRIC			5
# define TYPE_TORUS				6

# define SCREEN_EYE_DIST		400
# define MAX_RANGE				20000.0
# define DOESNOTEXIST			-2147483648.0



typedef struct 		s_quadric
{
	double a;
	double b;
	double c;
	double f;
	double g;
	double h;
	double p;
	double q;
	double r;
	double d;
}					t_quadric;
typedef struct		s_color
{
	double			red;
	double			green;
	double			blue;
}					t_color;

typedef struct		s_mat
{
	t_color			diffuse;
	double			transparency;
	double			refraction;
	double			reflection;
	double			brillance; // can be converted to t_color
//	int				perlin[512];
	double			bump;

	int				type;
	double			procedural_scale;
	t_color			diffuse2;
}					t_mat;


typedef struct		s_vec
{
	double			x;
	double			y;
	double			z;
}					t_vec;


typedef struct		s_ray
{
	t_vec			initialstart; // before any reflection/refraction
	t_vec			start;
	t_vec			dir;
	bool			indirect; //if the ray is reflected or refracted, is true
	int 			pixel_id;
	//not handled yet
}					t_ray;


typedef struct		s_objgpu
{
	int				id;
	//int 			parent;

	int				type;

	t_mat			material;
	t_vec			pos;
	t_vec			dir;
	double			rad;
	double			rad2; // small ring radius in torus
	double			height;
	double			alpha;
	t_quadric		quad;
	
	bool			isneg;

	double 			rotation; // rotation degree on axis
	int 			type_obj; // negative, slice, or object
}					t_objgpu;


typedef struct		s_objcomplement
{
	double			t[2]; //storage needed for negative objects
	bool			reversen;
	t_vec 			n;
//	void 			(*normal)(struct s_env *, struct s_obj *obj, struct s_objcomplement *comp);
//	t_objgpu		*normobj;
}					t_objcomplement;
/*
typedef struct		s_objgpu
{
	//int 			parent;

	int				type;
	t_mat			material;
	t_vec			pos;
	t_vec			dir;
	double			rad;
	double			height;
	double			alpha;
	t_quadric		quad;

	bool			reversen; //useful if we collide with the inside of an object
	double 			rotation; // rotation degree on axis
	double			t[2]; //storage needed for negative objects
	t_slice			nextslice[LIMIT_SLICE];
	int 			specificnormal;

	//in the case of a negative object (or eventually a composed one)
	//we need to have a varaible normal function
	//ex : negative sphere in cylinder will need
	//sphere normal calculation and object information

	//void 			(*normal)(struct s_env *, struct s_obj *obj);
	t_neg 			normobj;
	t_neg			nextneg[LIMIT_NEG];

	int				id;
	bool			set;
}					t_objgpu;*/

typedef struct 		s_color_data
{
	t_vec ray_dir;
	t_vec inter_point;
	t_vec n;
	t_mat cmat;
	double coef;
}					t_color_data;

typedef struct		s_lightgpu
{
	t_vec			pos;
	t_color			intensity;
	bool 			set;
}					t_lightgpu;

typedef struct 		s_gpu_out
{
	double 			t;
	int 			id;
	t_ray 			r;
	t_vec			normal;
	t_color			color;
}					t_gpu_out;


static double _root3 ( double x );
double root3 ( double x );
int SolveP3(double *x,double a,double b,double c);
void  CSqrt( double x, double y, double *a, double *b); // returns:  (*a);+i*s = sqrt(x+i*y);
int   SolveP4Bi(double *x, double b, double d);	// solve equation x^4 + b*x^2 + d = 0
static void  dblSort3( double *a, double *b, double *c); // make: a <= b <= c
int   SolveP4De(double *x, double b, double c, double d);	// solve equation x^4 + b*x^2 + c*x + d
double N4Step(double x, double a,double b,double c,double d);	// one Newton step for x^4 + a*x^3 + b*x^2 + c*x + d
int	SolveP4(double x[4],double a,double b,double c,double d);
void	swapdouble(double *a, double *b);

void	normalplane(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t);
void	normalsphere(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t);
void	normalcylinder(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t);
void	normalcone(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t);
void	normalquadric(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t);
void	normaltorus(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t);
void	normalget(__global t_ray *r, __global t_objgpu *obj, t_objcomplement *comp, double *t);

int		irayplane(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp);
int		iraycone(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp);
int		iraysphere(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp);
int		irayquadric(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp);
int		iraycylinder(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp);
int		iraytorus(__global t_ray *r, __global t_objgpu *obj, double *t0, t_objcomplement *comp);

int		irayslice(__global t_ray *r, __global t_objgpu *obj, double *dist, t_objcomplement *comp);
int		irayneg(__global t_ray *r, __global t_objgpu *obj, double *dist, t_objcomplement *comp);

static double 	fade(double t);
static double 	lerp(double t, double a, double b);
static double 	grad(int hash, double x, double y, double z);
void			init_perlin(int perlin[512]);
double 			noise(double x, double y, double z);


double		computeshadow(__global t_ray *r, __global t_objgpu *obj, double light, double dist);
void		perlinmap (t_vec inter_point, t_mat *cmat);
t_vec		bump_mapping(t_vec inter_point, t_mat *cmat, t_vec n);
void		marble (t_vec inter_point, t_mat *cmat);
void		checker (t_vec inter_point, t_mat *cmat);
t_color		deal_shadow(__global t_ray *lightray, __global t_objgpu *obj, __global t_lightgpu *lights, t_color_data *data);
void		blinn_phong(t_color *col, t_vec lightray_dir, t_color_data *data);
t_color		colorinit(double red, double green, double blue);


t_vec				vectoradd(t_vec v1, t_vec v2);
double				vectordot(t_vec v1, t_vec v2);
t_vec				vectorinit(double x, double y, double z);
t_vec				vectornormalize(t_vec *v);
t_vec				vectorproduct(t_vec u, t_vec v);
t_vec				vectorscale(double c, t_vec v);
t_vec				vectorscalediv(double c, t_vec v);
t_vec				vectorsub(t_vec v1, t_vec v2);
t_vec				vectordiv(t_vec u, t_vec v);
double				vectormagnitude(t_vec v);
t_vec				vectorrotate(t_vec v, t_vec axis, double angle);
t_vec 				vectorpointrotatearoundaxis(t_vec axp, t_vec axd, t_vec p, double theta);
