/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instersect_cylindre.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 14:24:21 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/18 18:19:53 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "ray_intersection.h"
#include "vector.h"
#include "math.h"

//http://heigeas.free.fr/laure/ray_tracing/cylindre.html

void	up_disk_cylindre(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	obj->p.disk.center = vec_add(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, obj->p.cylindre.hauteur / 2.0));
	obj->p.disk.normal = obj->p.cylindre.orientation;
	obj->p.disk.rayon = obj->p.cylindre.rayon;
	all_inter->up_disk = intersect_disk(obj, ray);
	if (all_inter)
		all_inter->rayhit_up_disk = obj->rayhit;
}

void	down_disk_cylindre(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	obj->p.disk.center = vec_add(obj->p.cylindre.center, vec_mul_scalar(obj->p.cylindre.orientation, (obj->p.cylindre.hauteur / 2.0) * -1.0));
	obj->p.disk.normal = obj->p.cylindre.orientation;
	obj->p.disk.rayon = obj->p.cylindre.rayon;
	all_inter->down_disk = intersect_disk(obj, ray);
	if (all_inter)
		all_inter->rayhit_down_disk = obj->rayhit;
}

int	give_intersec_normalize_cylinder(t_cylindre *new_cylindre, t_ray *new_ray, double *distance[2])
{
	double	delta;
	double	a;
	double	b;
	double	c;

	a = (new_ray->dir.y * new_ray->dir.y) + (new_ray->dir.z * new_ray->dir.z);
	b = 2 * ((new_ray->dir.y * new_ray->org.y) + (new_ray->dir.x * new_ray->org.x));
	c = (new_ray->org.y * new_ray->org.y) + (new_ray->org.z * new_ray->org.z) + (new_cylindre->rayon * new_cylindre->rayon);
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

bool intersect_paille_inf(t_cylindre_utils *all_inter, t_object *obj, t_ray *ray)
{
	t_cylindre	new_cylindre;
	t_ray		new_ray;
	double		distance[2];
	
	if (give_intersec_normalize_cylinder(&new_cylindre, &new_ray, &distance))
		;
	
}

bool	intersect_cylindre(t_object *obj, t_ray *ray)
{
	t_cylindre_utils all_inter;

	up_disk_cylindre(&all_inter, obj, ray);
	down_disk_cylindre(&all_inter, obj, ray);
	paille_intersec(&all_inter, obj, ray);
}