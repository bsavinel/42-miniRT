/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 17:58:59 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/21 17:10:06 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum e_object_type
{
	T_SPHERE,
	T_PLAN
}			t_object_type;

/* Math related structure */

typedef struct e_vec
{
	double	x;
	double	y;
	double	z;
}				t_vec;

typedef t_vec	t_vec3d;
typedef t_vec	t_3dpoint;
typedef t_vec	t_2dpoint;
typedef t_vec	t_albedo;
typedef t_vec	t_color;

/* minilibx and program structure */



/* Related ray structure */

typedef struct e_ray
{
	t_3dpoint	org;
	t_vec3d		dir;
}			t_ray;

typedef struct s_rayhit
{
	double		t;
	t_3dpoint	intersect_p;
	t_vec3d		normal;
}	t_rayhit;

/* Primitive geometry structure */

typedef struct e_sphere
{
	t_3dpoint	center;
	double		radius;
}				t_sphere;

typedef struct e_plan
{
	t_3dpoint	point;
	t_vec3d		normal;
}				t_plan;

typedef struct e_disk
{
	t_3dpoint	center;
	double		rayon;
	t_vec3d		normal;
}				t_disk;

typedef struct e_cylindre
{
	t_3dpoint	center;
	t_vec3d		orientation;
	double		rayon;  //* le parsing donne le diametre mais il me faut le rayon
	double		hauteur;
}				t_cylindre;

/*
 *	cylinder intersec struct
 */

typedef struct e_cylindre_utils
{
	bool		up_disk;
	t_rayhit	rayhit_up_disk;
	bool		down_disk;
	t_rayhit	rayhit_down_disk;
	bool		cylindre_first;
	t_rayhit	rayhit_cylindre_first;
	bool		cylindre_second;
	t_rayhit	rayhit_cylindre_second;
}				t_cylindre_utils;


typedef struct e_object	t_object;

typedef bool (*t_intersect_fnct)(t_object *, t_ray *);

struct e_object
{
	union
	{
		t_sphere	sphere;
		t_plan		plan;
		t_disk		disk;
		t_cylindre	cylindre;
	} p;
	t_albedo			albedo;
	t_rayhit			rayhit;
	t_object_type		type;
	t_intersect_fnct	fnct;
};

void	vec_print(t_vec a)
{
	printf("(%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
}

t_vec	vec(double x, double y, double z)
{
	t_vec	vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	return (vector);
}

t_vec	vec_add(t_vec a, t_vec b)
{
	return (vec(a.x + b.x, a.y + b.y, a.z + b.z));
}

t_vec	vec_sub(t_vec a, t_vec b)
{
	return (vec(a.x - b.x, a.y - b.y, a.z - b.z));
}

double	vec_dot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec	vec_mul_scalar(t_vec a, double scalar)
{
	return (vec(a.x * scalar, a.y * scalar, a.z * scalar));
}

double	vec_length(t_vec a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

void	vec_normalize_p(t_vec *a)
{
	double	lenght;

	lenght = 1 / vec_length(*a);
	a->x *= lenght;
	a->y *= lenght;
	a->z *= lenght;
}

t_vec	vec_normalize(t_vec a)
{
	double	lenght;

	lenght = 1 / vec_length(a);
	return (vec(a.x * lenght, a.y * lenght, a.z * lenght));
}

double	vec_lenght_p(t_vec a)
{
	return (a.x * a.x + a.y * a.y + a.z * a.z);
}

void	rotate_z(t_vec *point, double rotation)
{
	double	xtmp;
	double	ytmp;

	xtmp = point->x;
	ytmp = point->y;
	point->x = xtmp * cos(rotation) - ytmp * sin(rotation);
	point->y = xtmp * sin(rotation) + ytmp * cos(rotation);
}

void	rotate_y(t_vec *point, double rotation)
{
	double	xtmp;
	double	ztmp;

	xtmp = point->x;
	ztmp = point->z;
	point->x = xtmp * cos(rotation) + ztmp * sin(rotation);
	point->z = -xtmp * sin(rotation) + ztmp * cos(rotation);
}

void	rotate_x(t_vec *point, double rotation)
{
	double	ytmp;
	double	ztmp;

	ytmp = point->y;
	ztmp = point->z;
	point->y = ytmp * cos(rotation) - ztmp * sin(rotation);
	point->z = ytmp * sin(rotation) + ztmp * cos(rotation);
}


















/*void	normalize_cylinder(t_vec ob)
{
	
	double xylenght = sqrt(ob.x * ob.x + ob.y * ob.y);
	double zangle = acos(ob.y / xylenght);  // met x a 0
	
	
	double vecLength = sqrt(ob.x * ob.x + ob.y * ob.y + ob.z * ob.z);
	double xangle = acos(xylenght / vecLength);
	
	if (ob.x > 0)
		rotate_z(&ob, zangle);
	else
		rotate_z(&ob, zangle * -1.0);
	if (ob.z < 0)
		rotate_x(&ob, xangle);
	else
		rotate_x(&ob, xangle * -1.0);



	
	ob = vec_normalize(ob);
	printf("ob \nx :%lf, y: %lf z:%lf\n", ob.x, ob.y, ob.z);
}*/

void	normalize_cylinder(t_vec ob)
{
	//printf("start ob \nx :%lf, y: %lf z:%lf\n", ob.x, ob.y, ob.z);
	double xzlenght = sqrt(ob.x * ob.x + ob.z * ob.z);
	double yangle = acos(ob.x / xzlenght);  // met x a 0
	
	double vecLength = sqrt(ob.x * ob.x + ob.y * ob.y + ob.z * ob.z);
	double zangle = acos(xzlenght / vecLength);
		
	//rotate_y(&ob, yangle);
	//rotate_z(&ob, zangle);
	
	if (ob.z > 0)
		rotate_y(&ob, yangle);
	else
		rotate_y(&ob, yangle * -1.0);
	if (ob.y < 0)
		rotate_z(&ob, zangle);
	else
		rotate_z(&ob, zangle * -1.0);



	
	ob = vec_normalize(ob);
	printf("ob \nx :%lf, y: %lf z:%lf\n", ob.x, ob.y, ob.z);
}




/*
void	normalize_cylinder(t_vec ob)
{
	double	angle_y;
	double	angle_z;
	double	angle_x;
	
	printf("start ob \n x :%lf, y: %lf z:%lf\n", ob.x, ob.y, ob.z);
	angle_x = 1 - ob.x
	angle_y = 0 - ob.y
	angle_z = 0 - ob.z
	//angle_x = acos(vec_dot(vec(1, 0, 0), ob));
	//angle_y = acos(vec_dot(vec(0, 1, 0), ob));
	//angle_z = acos(vec_dot(vec(0, 0, 1), ob));
	
	rotate_y(&ob, angle_y);
	rotate_z(&ob, angle_z);
	//rotate_x(&ob, angle_x);
	ob = vec_normalize(ob);
	printf("end ob \n x :%lf, y: %lf z:%lf\n", ob.x, ob.y, ob.z);
}*/

int main()
{
	normalize_cylinder(vec_normalize(vec(0.7, 0.2, 0.5)));
	normalize_cylinder(vec_normalize(vec(0.7, 0.2, -0.5)));
	normalize_cylinder(vec_normalize(vec(0.7, -0.2, 0.5)));
	normalize_cylinder(vec_normalize(vec(0.7, -0.2, -0.5)));
	normalize_cylinder(vec_normalize(vec(-0.7, 0.2, 0.5)));
	normalize_cylinder(vec_normalize(vec(-0.7, 0.2, -0.5)));
	normalize_cylinder(vec_normalize(vec(-0.7, -0.2, 0.5)));
	normalize_cylinder(vec_normalize(vec(-0.7, -0.2, -0.5)));
	normalize_cylinder(vec_normalize(vec(0, 1, -0.5)));
	normalize_cylinder(vec_normalize(vec(-0.7, 0, 1)));
	normalize_cylinder(vec_normalize(vec(-0.7, 1, 0)));
}
