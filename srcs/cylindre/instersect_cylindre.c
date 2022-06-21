/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instersect_cylindre.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 14:24:21 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/21 17:28:30 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_utils.h"
#include "minirt_struct.h"
#include "ray_intersection.h"
#include "vector.h"
#include <math.h>

//http://heigeas.free.fr/laure/ray_tracing/cylindre.html

//? Refaire le plan pour les disque

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

	obj_tmp.p.disk.center = vec_add(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, (obj->p.cylindre.hauteur / 2.0) * -1.0));
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
	b = 2 * ((new_ray->dir.y * new_ray->org.y) + (new_ray->dir.x * new_ray->org.x));
	c = (new_ray->org.y * new_ray->org.y) + (new_ray->org.z * new_ray->org.z) + (rayon * rayon);
	delta = (b * b) - (4.0 * a * c);
	if (delta == 0.0)
	{
		distance[0] = (-1 * b) / (2 * a);
		return (1);
	}
	else
	{
		distance[0] = ((-1 * b) - sqrt(delta)) / (2 * a);
		distance[1] = ((-1 * b) + sqrt(delta)) / (2 * a);
		return (2);
	}
	return (0);
}

void	normalize_cylinder(t_object *obj, t_ray *ray, t_ray *new_ray, t_cylindre_utils *all_inter)
{
	double xzlenght;
	double vecLength;
	
	xzlenght = sqrt(obj->p.cylindre.orientation.x * obj->p.cylindre.orientation.x + obj->p.cylindre.orientation.z * obj->p.cylindre.orientation.z);
	all_inter->yangle = acos(obj->p.cylindre.orientation.x / xzlenght);
	vecLength = sqrt(obj->p.cylindre.orientation.x * obj->p.cylindre.orientation.x + obj->p.cylindre.orientation.y * obj->p.cylindre.orientation.y + obj->p.cylindre.orientation.z * obj->p.cylindre.orientation.z);
	all_inter->zangle = acos(xzlenght / vecLength);
	if (obj->p.cylindre.orientation.z <= 0)
		all_inter->yangle = all_inter->yangle * -1.0;
	if (obj->p.cylindre.orientation.y >= 0)
		all_inter->zangle = all_inter->zangle * -1.0;		
	new_ray->dir = ray->dir;
	rotate_y(&new_ray->dir, all_inter->yangle);
	rotate_z(&new_ray->dir, all_inter->zangle);
	new_ray->dir = vec_normalize(new_ray->dir);
	new_ray->org = vec_sub(ray->org, obj->p.cylindre.center);
}


t_vec	unnormalize_cynlinder(t_vec	normalize_normal, t_cylindre_utils *all_inter)
{
	rotate_y(&normalize_normal, all_inter->yangle * -1.0);
	rotate_z(&normalize_normal, all_inter->zangle * -1.0);
	return (normalize_normal);
}

void	intersect_paille_inf(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	t_ray		new_ray;
	double		distance[2];
	int			tmp;

	normalize_cylinder(obj, ray, &new_ray, all_inter);
	tmp = give_intersec_normalize_cylinder(obj->p.cylindre.rayon, &new_ray, distance);
	if (tmp > 0)
	{
		all_inter->rayhit_cylindre_first.t = distance[0];
		all_inter->rayhit_cylindre_first.intersect_p = vec_add(ray->org, vec_mul_scalar(ray->dir, distance[0]));
		all_inter->rayhit_cylindre_second.normal = unnormalize_cynlinder(vec_normalize(vec(0, all_inter->rayhit_cylindre_first.intersect_p.y, all_inter->rayhit_cylindre_first.intersect_p.x)), all_inter);
		all_inter->cylindre_first = true;
	}
	if (tmp == 2)
	{
		all_inter->rayhit_cylindre_second.t = distance[1];
		all_inter->rayhit_cylindre_second.intersect_p = vec_add(ray->org, vec_mul_scalar(ray->dir, distance[1]));
		all_inter->rayhit_cylindre_second.normal = unnormalize_cynlinder(vec_normalize(vec(0, all_inter->rayhit_cylindre_second.intersect_p.y, all_inter->rayhit_cylindre_second.intersect_p.x)), all_inter);
		all_inter->cylindre_second = true;
	}
}

bool	check_individual_point(t_rayhit *rayhit, t_object *obj)
{
	t_object	tmp;
	t_ray		fake_ray;
	int			signe;

	signe = 0;
	fake_ray.org = rayhit->intersect_p;
	fake_ray.dir = obj->p.cylindre.orientation;
	tmp.p.plan.point = vec_add(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, obj->p.cylindre.hauteur / 2.0));
	tmp.p.plan.normal = obj->p.cylindre.orientation;
	plane_intersection(&tmp, &fake_ray);
	if (tmp.rayhit.t == 0)
		return (true);
	else if (tmp.rayhit.t > 0)
		signe++;
	else
		signe--;
	tmp.p.plan.point = vec_sub(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, obj->p.cylindre.hauteur / 2.0));
	plane_intersection(&tmp, &fake_ray);
	if (tmp.rayhit.t == 0 || (signe == 1 && tmp.rayhit.t < 0) || (signe == -1 && tmp.rayhit.t > 0))
		return (true);
	else
		return (false);
}

void	cut_paille_inf(t_cylindre_utils *all_inter, t_object *obj)
{
	if (all_inter->cylindre_first)
		all_inter->cylindre_first = check_individual_point(&all_inter->rayhit_cylindre_first, obj);
	if (all_inter->cylindre_second)
		all_inter->cylindre_second = check_individual_point(&all_inter->rayhit_cylindre_second, obj);
}

bool	select_first_intersect(t_cylindre_utils *all_inter, t_object *obj)
{
	bool		retour;
	t_rayhit	select;

	retour = false;
	if (all_inter->cylindre_first)
	{
		retour = true;
		select = all_inter->rayhit_cylindre_first;
	}
	if (all_inter->cylindre_second && (retour == false || select.t <= all_inter->rayhit_cylindre_second.t))
	{
		retour = true;
		select = all_inter->rayhit_cylindre_second;
	}
	if (all_inter->up_disk && (retour == false || select.t <= all_inter->rayhit_up_disk.t))
	{
		retour = true;
		select = all_inter->rayhit_up_disk;
	}
	if (all_inter->down_disk && (retour == false || select.t <= all_inter->rayhit_down_disk.t))
	{
		retour = true;
		select = all_inter->rayhit_down_disk;
	}
	obj->rayhit = select;
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
