/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instersect_cylindre.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 14:24:21 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/24 14:58:27 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_utils.h"
#include "minirt_struct.h"
#include "ray_intersection.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>

//http://heigeas.free.fr/laure/ray_tracing/cylindre.html

void	up_disk_cylindre(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	t_object	obj_tmp;

	obj_tmp.p.disk.center = vec_add(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, obj->p.cylindre.hauteur / 2.0));
	obj_tmp.p.disk.normal = obj->p.cylindre.orientation;
	obj_tmp.p.disk.rayon = obj->p.cylindre.rayon;
	all_inter->up_disk = intersect_disk(&obj_tmp, ray);
	if (all_inter->up_disk)
		all_inter->rayhit_up_disk = obj_tmp.rayhit;
}

void	down_disk_cylindre(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	t_object	obj_tmp;

	obj_tmp.p.disk.center = vec_sub(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, (obj->p.cylindre.hauteur / 2.0)));
	obj_tmp.p.disk.normal = obj->p.cylindre.orientation;
	obj_tmp.p.disk.rayon = obj->p.cylindre.rayon;
	all_inter->down_disk = intersect_disk(&obj_tmp, ray);
	if (all_inter->down_disk)
		all_inter->rayhit_down_disk = obj_tmp.rayhit;
}

int	give_intersec_normalize_cylinder(double rayon, t_ray *new_ray, double distance[2])
{
	double	delta;
	double	a;
	double	b;
	double	c;

	a = (new_ray->dir.y * new_ray->dir.y) + (new_ray->dir.z * new_ray->dir.z);
	b = 2 * ((new_ray->dir.y * new_ray->org.y) + (new_ray->dir.z * new_ray->org.z));
	c = (new_ray->org.y * new_ray->org.y) + (new_ray->org.z * new_ray->org.z) - (rayon * rayon);
	delta = (b * b) - (4.0 * a * c);
	if (delta == 0.0)
	{
		distance[0] = (-1.0 * b) / (2.0 * a);
		return (1);
	}
	else if (delta > 0.0)
	{
		distance[0] = ((-1.0 * b) - sqrt(delta)) / (2.0 * a);
		distance[1] = ((-1.0 * b) + sqrt(delta)) / (2.0 * a);
		//printf("t1 : %lf t2 :%lf\n", distance[0], distance[1]);
		return (2);
	}
	return (0);
}

void	normalize_cylinder(t_object *obj, t_ray *ray, t_ray *new_ray, t_cylindre_utils *all_inter)
{
	double	xzlenght;
	double	veclength;

	xzlenght = sqrt(obj->p.cylindre.orientation.x * obj->p.cylindre.orientation.x + obj->p.cylindre.orientation.z * obj->p.cylindre.orientation.z);
	all_inter->yangle = acos(obj->p.cylindre.orientation.x / xzlenght);
	veclength = sqrt(obj->p.cylindre.orientation.x * obj->p.cylindre.orientation.x + obj->p.cylindre.orientation.y * obj->p.cylindre.orientation.y + obj->p.cylindre.orientation.z * obj->p.cylindre.orientation.z);
	all_inter->zangle = acos(xzlenght / veclength);
	if (obj->p.cylindre.orientation.z < 0)
		all_inter->yangle = all_inter->yangle * -1.0;
	if (obj->p.cylindre.orientation.y > 0)
		all_inter->zangle = all_inter->zangle * -1.0;
	new_ray->dir = ray->dir;
	rotate_y(&new_ray->dir, all_inter->yangle);
	rotate_z(&new_ray->dir, all_inter->zangle);
	new_ray->dir = vec_normalize(new_ray->dir);
	new_ray->org = vec_sub(ray->org, obj->p.cylindre.center);
	all_inter->fake_ray = *new_ray;
	//printf("ray: \n x: %lf y: %lf z: %lf\nobj: \n x: %lf y: %lf z: %lf\nnew->ray: \n x: %lf y: %lf z: %lf\n\n", ray->org.x, ray->org.y, ray->org.z, obj->p.cylindre.center.x, obj->p.cylindre.center.y, obj->p.cylindre.center.z, new_ray->org.x, new_ray->org.y, new_ray->org.z);
}

t_vec	unnormalize_cynlinder(t_vec	normalize_normal, t_cylindre_utils *all_inter)
{
	rotate_z(&normalize_normal, all_inter->zangle * -1.0);
	rotate_y(&normalize_normal, all_inter->yangle * -1.0);
	normalize_normal = vec_normalize(normalize_normal);
	return (normalize_normal);
}

void	intersect_paille_inf(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	t_ray		new_ray;
	double		distance[2];
	t_3dpoint	tmp_point;
	int			tmp;

	normalize_cylinder(obj, ray, &new_ray, all_inter);
	tmp = give_intersec_normalize_cylinder(obj->p.cylindre.rayon, &new_ray, distance);
	if (tmp != 0)
	{
		all_inter->rayhit_cylindre_first.t = distance[0];
		all_inter->rayhit_cylindre_first.intersect_p = vec_add(ray->org, vec_mul_scalar(ray->dir, distance[0]));
		tmp_point = vec_add(new_ray.org, vec_mul_scalar(new_ray.dir, distance[0]));
		all_inter->rayhit_cylindre_second.normal = unnormalize_cynlinder(vec_normalize(vec(0, tmp_point.y, tmp_point.z)), all_inter);
		all_inter->cylindre_first = true;
	}
	if (tmp == 2)
	{
		all_inter->rayhit_cylindre_second.t = distance[1];
		all_inter->rayhit_cylindre_second.intersect_p = vec_add(ray->org, vec_mul_scalar(ray->dir, distance[1]));
		tmp_point = vec_add(new_ray.org, vec_mul_scalar(new_ray.dir, distance[1]));
		all_inter->rayhit_cylindre_second.normal = unnormalize_cynlinder(vec_normalize(vec(0,tmp_point.y, tmp_point.z)), all_inter);
		all_inter->cylindre_second = true;
	}
}

bool	check_individual_point(t_rayhit *rayhit, t_object *obj, t_cylindre_utils *all_inter)
{
	double	lenght;
	double	hauteur;

	(void)all_inter;
	return (true);
	lenght = vec_length(vec_sub(obj->p.cylindre.center, rayhit->intersect_p));
	hauteur = sqrt(-1.0 * (obj->p.cylindre.rayon * obj->p.cylindre.rayon) + (lenght * lenght));
	if (hauteur > obj->p.cylindre.hauteur / 2.0)
		return (false);
	return (true);
	/*t_3dpoint p;
	
	p = vec_add(all_inter->fake_ray.org, vec_mul_scalar(all_inter->fake_ray.dir, rayhit->t));
	if (fabs(p.x) > obj->p.cylindre.hauteur)
		return (false);
	return (true);*/
}

void	cut_paille_inf(t_cylindre_utils *all_inter, t_object *obj)
{
	if (all_inter->cylindre_first)
		all_inter->cylindre_first = check_individual_point(&all_inter->rayhit_cylindre_first, obj, all_inter);
	if (all_inter->cylindre_second)
		all_inter->cylindre_second = check_individual_point(&all_inter->rayhit_cylindre_second, obj, all_inter);
}

bool	select_first_intersect(t_cylindre_utils *all_inter, t_object *obj)
{
	bool		retour;

	retour = false;
	if (all_inter->cylindre_first)
	{
		retour = true;
		obj->rayhit = all_inter->rayhit_cylindre_first;
	}
	if (all_inter->cylindre_second && (retour == false || obj->rayhit.t > all_inter->rayhit_cylindre_second.t))
	{
		retour = true;
		obj->rayhit = all_inter->rayhit_cylindre_second;
	}
	if (all_inter->up_disk && (retour == false || obj->rayhit.t > all_inter->rayhit_up_disk.t))
	{
		retour = true;
		obj->rayhit = all_inter->rayhit_up_disk;
	}
	if (all_inter->down_disk && (retour == false || obj->rayhit.t > all_inter->rayhit_down_disk.t))
	{
		retour = true;
		obj->rayhit = all_inter->rayhit_down_disk;
	}
	return (retour);
}

bool	intersect_cylindre(t_object *obj, t_ray *ray)
{
	t_cylindre_utils	all_inter;

	all_inter.up_disk = false;
	all_inter.down_disk = false;
	all_inter.cylindre_first = false;
	all_inter.cylindre_second = false;
	up_disk_cylindre(&all_inter, obj, ray);
	down_disk_cylindre(&all_inter, obj, ray);
	intersect_paille_inf(&all_inter, obj, ray);
	cut_paille_inf(&all_inter, obj);
	return (select_first_intersect(&all_inter, obj));
}
