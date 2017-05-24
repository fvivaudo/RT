/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/07 11:39:05 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/17 16:20:19 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H


//(A, H, G, P)
//(H, B, F, Q)   ()
//(G, F, C, R) * ()
//(P, Q, R, D)   ()
# include <mlx.h>
# include <unistd.h>
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <libft.h>
# include <get_next_line.h>
# include <pthread.h>
# include <time.h>
# include <SDL2/SDL.h>
# include <SDL_image.h>
# include <stdbool.h>
# include <OpenCL/opencl.h>

#define PROCEDURAL_SCALE 0.05
#define	TwoPi  6.28318530717958648

# define LIMIT_OBJ				50

# define TRUE					1
# define FALSE					0
# define C						255.0

# define WIDTH					800
# define HEIGHT					600
# define KEPSILON				0.0001
# define FOV					120
# define MAX_RANGE				20000.0
# define MAX_DEPTH_LEVEL		60
# define AMBIANT_SHADING		0.15
# define SPEC_POW				20.0
# define EXPOSURE				-1

# define MAX_THREAD				1

# define TEXTURE_MARBLE			1
# define TEXTURE_CHECKER		2
# define TEXTURE_PERLIN		 	3


# define TYPE_SPHERE			1
# define TYPE_PLANE				2
# define TYPE_CYLINDER			3
# define TYPE_CONE				4
# define TYPE_QUADRIC			5
# define TYPE_TORUS				6

# define TYPE_OBJECT			1
# define TYPE_SLICE				2
# define TYPE_NEGATIVE			3

# define TYPE_BUMP				1
# define SCREEN_EYE_DIST		400
# define OHAO &o->obj->nextneg
//cosinus and sinus sometimes give back very small values instead of 0
//so i'm using this to convert them to 0
# define ROUNDING_LIMIT			0.000001

// if there is only one soluction to an intersection
# define DOESNOTEXIST			-2147483648.0

# define COSCALC (u.fdensity2 * u.fCosThetaT + u.fdensity1 * u.fCosThetaI)
# define COSSCALC (u.fdensity1 * u.fCosThetaT - u.fdensity2 * u.fCosThetaI)

struct					s_env;

typedef struct 			s_quadric
{
	double 				a;
	double 				b;
	double 				c;
	double 				f;
	double 				g;
	double 				h;
	double 				p;
	double 				q;
	double 				r;
	double 				d;
}						t_quadric;

typedef struct 			s_thread_task
{
	int					i;
    void 				*arg;
}						t_thread_task;

typedef struct			s_coord
{
	int					x0;
	int					y0;
	int					x1;
	int					y1;
	int					sx;
	int					sy;
	int					dx;
	int					dy;
	int					err;
	int					e2;
}						t_coord;

typedef struct			s_vec
{
	double				x;
	double				y;
	double				z;
}						t_vec;
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
typedef struct			s_ray
{
	t_vec				initialstart; // before any reflection/refraction
	t_vec				start;
	t_vec				dir;
	bool				indirect; //if the ray is reflected or refracted, is true
	int 				pixel_id;
	//not handled yet
}						t_ray;

typedef struct			s_color
{
	double				red;
	double				green;
	double				blue;


}						t_color;

typedef struct			s_light
{
	t_vec				pos;
	t_color				intensity;
	struct s_light		*next;
}						t_light;

typedef struct			s_lightgpu
{
	t_vec				pos;
	t_color				intensity;
	bool 				set;
}						t_lightgpu;

typedef struct			s_mat
{
	t_color				diffuse;
	double				transparency;
	double				refraction;
	double				reflection;
	double				brillance; // can be converted to t_color
//	int					perlin[512];
	double				bump;

	int					type;
	double				procedural_scale;
	t_color				diffuse2;
}						t_mat;


typedef struct			s_obj
{
	int					id;
	//int 			parent;

	int					type;

	t_mat				material;
	t_vec				pos;
	t_vec				dir;
	double				rad;
	double				rad2; // small ring radius in torus
	double				height;
	double				alpha;
	t_quadric			quad;

	bool				isneg;

	double 				rotation; // rotation degree on axis
	int 				type_obj; // negative, slice, or object

	struct s_obj		*nextitem;
	struct s_obj		*nextchild;
	struct s_obj		*nextslice;
	struct s_obj		*nextneg;
	//in the case of a negative object (or eventually a composed one)
	//we need to have a variable normal function
	//ex : negative sphere in cylinder will need
	//sphere normal calculation and object information
	//reversen, t[2], normal and normobj need to be read only or exported
}						t_obj;

//parent is an object containing only a position, orientation, and child objects
//orientation and position of child objects become local
typedef struct			s_objgpu
{
	int					id;
	//int 			parent;

	int					type;

	t_mat				material;
	t_vec				pos;
	t_vec				dir;
	double				rad;
	double				rad2; // small ring radius in torus
	double				height;
	double				alpha;
	t_quadric			quad;

	bool				isneg;

	double 				rotation; // rotation degree on axis
	int 				type_obj; // negative, slice, or object
}						t_objgpu;



typedef struct			s_objcomplement
{
	double				t[2]; //storage needed for negative objects
	bool				reversen;
	t_vec 				n;
	void 				(*normal)(struct s_env *, struct s_obj *obj, struct s_objcomplement *comp);
	struct s_obj 		*normobj;
}						t_objcomplement;

typedef struct 			s_cam
{
	t_vec				xincvector;
	t_vec				yincvector;
	t_vec				eyepoint;
	t_vec				lookat;
	t_vec				viewplanebottomleftpoint;
	t_vec				vdir;
}						t_cam;

typedef struct			s_env
{
	void				*win;
	void				*ima;

/*
	gpu related
*/	t_ray 				*gpu_in;
	int 				pix_rendu;

	double 				effect;
	double				ered;
	double				egreen;
	double				eblue;

	t_color				col;

	int					level;
	double				coef;
	double				t;
	int					csphere;
//	int					cobj;
	double				temp;
	unsigned int		i;
	unsigned int		j;
	t_ray				r;
	t_light				*lights;
	t_light				clight;
	t_vec				dist; //temporary
	t_ray				lightray;
	t_mat				cmat; //current material
	t_vec				scaled; //temporary
	t_vec				newstart; //intersection point with an object
	t_vec				n; //vector normal
	t_obj				*obj; //start of object list
	t_vec				vdir;
	int					id; //new object id
	double				lambert;
	double				reflect;
	t_vec				tmp;
	t_cam				cam;
	int					x;
	int					y;

	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	pthread_mutex_t		mutex2;
	int					available_ressource;

	double				crefraction;
	double				transcoef;
	double				reflecoef;
}						t_env;

typedef struct 			s_gpu_out
{
	double 				t;
	int 				id;
	t_ray 				r;
	t_vec				normal;
	t_color				color;
}						t_gpu_out;

typedef struct 			s_gputool
{
	int 				pix_rendu;
	t_ray 				*gpu_in;
	t_gpu_out 			*gpu_out;
	t_objgpu			*gpu_in_objs;
	t_lightgpu			*gpu_in_lights;
	int 				sizeobj;
	int 				sizelight;
}						t_gputool;

typedef struct 			s_cl
{
	char 				*source;
	cl_device_id 		device_id;
	cl_context 			context;
	cl_command_queue 	commands;
	cl_program 			program;
	cl_kernel 			kernel;
	int 				err;
	cl_mem 				input;
	cl_mem 				output;
	cl_mem 				objs;
	cl_mem 				lights;
	size_t 				local;
	size_t 				global;
	unsigned int 		count;
}						t_cl;


typedef struct 			s_sray_info
{
	double 				coef;
	double 				previousrefractionindex;
}						t_sray_info;

void				cast_ray(t_env *e);
t_color				colorinit(double red, double green, double blue);
t_obj				*computeray(t_env *e);
int					deal_shadow(t_env *e);

int 				init_effect(t_env *e,char **buffer);
t_env				*readConfig(int fd);
t_obj 				*init_null(void);
bool				setnegative(char **buffer, int *y, t_obj *parent, t_obj **lstobj);
bool				setslice(char **buffer, int *y, t_obj *obj);
bool				setorient(char **buffer, int *y, t_obj *obj);
bool			 	setmat(char **buffer, int *y, t_mat *mat);
void				extractobj(t_obj **lstobj, t_obj *obj, int id);

void				init_cam(t_env *e, char **buffer);
int					init_cone(t_obj **lstobj, char **buffer, bool neg);
int					init_cyl(t_obj **lstobj, char **buffer, bool neg);
void				init_light(t_env *e, char **buffer);
int					init_plane(t_obj **lstobj, char **buffer, bool neg);
int					init_sphere(t_obj **lstobj, char **buffer, bool neg);
int					init_quadric(t_obj **lstobj, char **buffer, bool neg);
int					init_torus(t_obj **lstobj, char **buffer, bool neg);
void				init_compose(t_obj **lstobj, char **buffer);
int					init_object(t_obj **lstobj, char **buffer, bool neg);


t_obj				*intersection(t_env *e, t_ray *r, t_objcomplement *comp);
int					iraycone(t_ray *r, t_obj *co, double *t0, t_objcomplement *comp);
int					iraycone2(double abcd[4], double t[2], double *t0);
int					iraycylinder(t_ray *r, t_obj *cyl, double *t0, t_objcomplement *comp);
int					irayplane(t_ray *r, t_obj *p, double *t0, t_objcomplement *comp);
int					iraysphere(t_ray *r, t_obj *s, double *t0, t_objcomplement *comp);
int					irayquadric(t_ray *r, t_obj *obj, double *t0, t_objcomplement *comp);
int					iraytorus(t_ray *r, t_obj *obj, double *t0, t_objcomplement *comp);

int					irayneg(t_ray *r, t_obj *obj, double *dist, t_objcomplement *comp);
int					irayslice(t_ray *r, t_obj *obj, double *dist, t_objcomplement *comp);

void				print_img(unsigned char img[3 * WIDTH * HEIGHT]);
void				reset(t_env *e, int x, int y);
unsigned char		*update_img(t_color *col, int pixel_id, double coef);

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

double				computeshadow(t_env *e, t_ray *r, double light, double dist);

double				noise(double x, double y, double z);
t_quadric			quadricrotate(t_quadric to_rot, t_vec r_a, double rad, t_vec pos);

void				lstaddneg(t_obj **alst, t_obj *new);
void				lstaddobj(t_obj **alst, t_obj *new);
void				lstaddslice(t_obj **alst, t_obj *new);
void				lstaddlight(t_light **alst, t_light *new);
t_obj				*lstremoveoneobj(t_obj **alst, int id);

void				normalsphere(t_env *e, t_obj *obj, t_objcomplement *comp);
void				normalplane(t_env *e, t_obj *obj, t_objcomplement *comp);
void				normalcylinder(t_env *e, t_obj *obj, t_objcomplement *comp);
void				normalcone(t_env *e, t_obj *obj, t_objcomplement *comp);
void				normalquadric(t_env *e, t_obj *obj, t_objcomplement *comp);
void				normaltorus(t_env *e, t_obj *obj, t_objcomplement *comp);



void				swapdouble(double *a, double *b);

t_vec				bump_mapping(t_env *e);
void				blinn_phong(t_env *e, t_vec lightray_dir);

#endif
