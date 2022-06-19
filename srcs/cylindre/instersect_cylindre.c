/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instersect_cylindre.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 14:24:21 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/19 12:33:20 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "ray_intersection.h"
#include "vector.h"
#include "math.h"

//http://heigeas.free.fr/laure/ray_tracing/cylindre.html

//? Refaire le plan pour les disque

void	up_disk_cylindre(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	t_object obj_tmp;
	
	obj_tmp.p.disk.center = vec_add(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, obj->p.cylindre.hauteur / 2.0));
	obj_tmp.p.disk.normal = obj->p.cylindre.orientation;
	obj_tmp.p.disk.rayon = obj->p.cylindre.rayon;
	all_inter->up_disk = intersect_disk(&obj_tmp, ray);
	if (all_inter->up_disk)
		all_inter->rayhit_up_disk = obj_tmp.rayhit;
}

void	down_disk_cylindre(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	t_object obj_tmp;

	obj_tmp.p.disk.center = vec_add(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, (obj->p.cylindre.hauteur / 2.0) * -1.0));
	obj_tmp.p.disk.normal = obj->p.cylindre.orientation;
	obj_tmp.p.disk.rayon = obj->p.cylindre.rayon;
	all_inter->down_disk = intersect_disk(&obj_tmp, ray);
	if (all_inter->down_disk)
		all_inter->rayhit_down_disk = obj_tmp.rayhit;
}

int	give_intersec_normalize_cylinder(double rayon, t_ray *new_ray, double *distance[2])
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
		(*distance)[0] = (-1 * b) / (2 * a);
		return (1);
	}
	else
	{
		(*distance)[0] = ((-1 * b) - sqrt(delta)) / (2 * a);
		(*distance)[1] = ((-1 * b) + sqrt(delta)) / (2 * a);
		return (2);
	}
	return (0);
}

void	normalize_cylinder(t_object *obj, t_ray *ray, t_ray *new_ray)
{
	new_ray->org = vec_sub(ray->org, obj->p.cylindre.center);
	//! //BUG C'EST DE LA MERDE 
	new_ray->dir = vec(ray->dir.x + (1 - obj->p.cylindre.orientation.x), ray->dir.y - obj->p.cylindre.orientation.y, ray->dir.z- obj->p.cylindre.orientation.z);
	new_ray->dir = vec_normalize(new_ray->dir);
}

// TODO Mettre les normales utiliser inverses au dessus car la normal est x=0 donc reatribue l'inverse lors du remplacement

void	intersect_paille_inf(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	t_ray		new_ray;
	double		distance[2];
	int			tmp;
	
	normalize_cylinder(obj, ray, &new_ray);
	tmp = give_intersec_normalize_cylinder(obj->p.cylindre.rayon, &new_ray, &distance);
	if (tmp > 0)
	{
		all_inter->rayhit_cylindre_first.t = distance[0];
		all_inter->rayhit_cylindre_first.intersect_p = vec_add(ray->org, vec_mul_scalar(ray->dir, distance[0]));
		all_inter->cylindre_first = true;
	}
	if (tmp == 2)
	{
		all_inter->rayhit_cylindre_second.t = distance[1];
		all_inter->rayhit_cylindre_second.intersect_p = vec_add(ray->org, vec_mul_scalar(ray->dir, distance[1]));
		all_inter->cylindre_second = true;
	}
}

bool	check_individual_point(t_rayhit *rayhit, t_object *obj)
{
	
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
	return (retour);
}

bool	intersect_cylindre(t_object *obj, t_ray *ray)
{
	t_cylindre_utils all_inter;

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