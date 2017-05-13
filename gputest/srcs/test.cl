
# define SIZE_ 					480000
# define EFFECT 				0
# define RED 					30
# define GREEN 					15
# define BLUE 					5

#pragma OPENCL EXTENSION cl_khr_fp64 : enable
# define TRUE					1
# define FALSE					0
# define C						255.0

# define WIDTH					800
# define HEIGHT					600
# define KEPSILON				0.0001
# define FOV					120
# define MAX_RANGE				20000.0
# define MAX_DEPTH_LEVEL		5
# define AMBIANT_SHADING		0.15
# define SPEC_POW				20.0
# define EXPOSURE				-1

# define MAX_THREAD				16

# define TYPE_SPHERE			11
# define TYPE_PLANE				2
# define TYPE_CYLINDER			3
# define TYPE_CONE				4
# define TYPE_QUADRIC			5

# define TYPE_BUMP				1
# define SCREEN_EYE_DIST		400

//cosinus and sinus sometimes give back very small values instead of 0
//so i'm using this to convert them to 0
# define ROUNDING_LIMIT			0.000001

// if there is only one soluction to an intersection
# define DOESNOTEXIST			-2147483648.0

struct		s_env;


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

typedef struct 		s_thread_task
{
	int				i;
    //void 			*arg;
}					t_thread_task;

typedef struct		s_coord
{
	int				x0;
	int				y0;
	int				x1;
	int				y1;
	int				sx;
	int				sy;
	int				dx;
	int				dy;
	int				err;
	int				e2;
}					t_coord;

typedef struct		s_vec
{
	double			x;
	double			y;
	double			z;
}					t_vec;
/*
typedef struct		s_plane
{
	t_vec			dir;
	t_vec			pos;
}					t_plane;

typedef struct		s_sphere
{
	t_vec			pos;
	double			rad;
}					t_sphere;

typedef struct		s_cylinder
{
	t_vec			dir;
	t_vec			pos;
	double			height;
	double			rad;
}					t_cylinder;

typedef struct		s_cone
{
	t_vec			dir;
	t_vec			pos;
	double			rad;
	double			alpha;
}					t_cone;
*/
typedef struct		s_ray
{
	t_vec			initialstart; // before any reflection/refraction
	t_vec			start;
	t_vec			dir;
	bool			indirect; //if the ray is reflected or refracted, is true
	//not handled yet
}					t_ray;

typedef struct		s_color
{
	double			red;
	double			green;
	double			blue;
}					t_color;

typedef struct		s_light
{
	t_vec			pos;
	t_color			intensity;
//	struct s_light	*next;
}					t_light;

typedef struct		s_mat
{
	int				type;
	t_color			diffuse;
	t_color			diffuse2;
	double			transparency;
	double			refraction;
	double			reflection;
	double			brillance; // can be converted to t_color
	int				perlin[512];
	double			bump;
}					t_mat;

//parent is an object containing only a position, orientation, and child objects
//orientation and position of child objects become local
typedef struct		s_obj
{
	int				id;
	//int 			parent;

	int				type;

	//struct s_obj	*nextitem;
	//struct s_obj	*nextchild;
	//struct s_obj	*nextslice;
	//struct s_obj	*nextneg;
	t_mat			material;
	t_vec			pos;
	t_vec			dir;
	double			rad;
	double			height;
	double			alpha;
	t_quadric		quad;

	bool			isneg;
	bool			reversen; //useful if we collide with the inside of an object

	double 			rotation; // rotation degree on axis

	double			t[2]; //storage needed for negative objects

	//in the case of a negative object (or eventually a composed one)
	//we need to have a varaible normal function
	//ex : negative sphere in cylinder will need
	//sphere normal calculation and object information
	//void 			(*normal)(struct s_env *, struct s_obj *obj);
	//struct s_obj 	*normobj;
}					t_obj;


typedef struct 		s_cam
{
	t_vec			xincvector;
	t_vec			yincvector;
	t_vec			eyepoint;
	t_vec			lookat;
	t_vec			viewplanebottomleftpoint;
	t_vec			vdir;
}					t_cam;

typedef struct		s_env
{
	//void			*win;
	//void			*ima;
	double			effect;

	t_color			col;

	int				level;
	double			coef;
	double			t;
	int				csphere;
//	int				cobj;
	double			temp;
	unsigned int	i;
	unsigned int	j;
	t_ray			r;
	//t_light			*lights;
	t_light			clight;
	t_vec			dist; //temporary
	t_ray			lightray;
	t_mat			cmat; //current material
	t_vec			scaled; //temporary
	t_vec			newstart; //intersection point with an object
	t_vec			n; //vector normal
	//t_obj			*obj; //start of object list
	t_vec			vdir;
	int				id; //new object id
	double			lambert;
	double			reflect;
	t_vec			tmp;
	t_cam			cam;
	int				x;
	int				y;

	int				available_ressource;

	double			crefraction;
	double			refracoef;
	double			reflecoef;
	double			transcoef;
}					t_env;
typedef struct		s_envv
{
//	void			*win;
//	void			*ima;
	double			effect;

	t_color			col;

	int				level;
	double			coef;
	double			t;
	int				csphere;
//	int				cobj;
	double			temp;
	unsigned int	i;
	unsigned int	j;
	t_ray			r;
	//t_light			*lights;
	t_light			clight;
	t_vec			dist; //temporary
	t_ray			lightray;
	t_mat			cmat; //current material
	t_vec			scaled; //temporary
	t_vec			newstart; //intersection point with an object
	t_vec			n; //vector normal
	//t_obj			*obj; //start of object list
	t_vec			vdir;
	int				id; //new object id
	double			lambert;
	double			reflect;
	t_vec			tmp;
	t_cam			cam;
	int				x;
	int				y;

	int				available_ressource;

	double			crefraction;
	double			refracoef;
	double			reflecoef;
	double			transcoef;
}					t_envv;

__kernel void actionn(__global float *input, __global float *output ,const unsigned int count){
	/*
		x = i % HEIGHT
		y = i / HEIGHT
	*/
	 int i = get_global_id(0);
	 float testb = 55.224;
	 float testc = 48.2555;
	 float testd = 12.44;
	 float teste = 65.12;
	 float testf = 88.999;
	 float testg = 48.11;
	 float testh = 0.022;
	 for (int y = 0;y<5000;y++){
		 if (i < count){
			double testa = sin(0.45);
		 	printf("%f\n", testa);
			testb = 55.224;
			testc = 48.2555;
			testd = 12.44;
			teste = 65.12;
			testf = 88.999;
			testg = 48.11;
		 	testh = 0.022;

			testa *= testb - i + y ;
			testb /= testc - i + y ;
			testd /= testa - i + y ;
			teste *= testh - i + y ;
			testf /= testb - i + y ;
			testg *= testh - i + y ;
			testh /= testa - i + y ;
			output[i] = testa+testb+testc+testd+teste+testf+testg+testh;
		}
	}
}
