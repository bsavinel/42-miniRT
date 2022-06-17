/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instersect_cylindre.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 14:24:21 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/17 16:55:57 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "ray_intersection.h"
#include "vector.h"

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

bool	intersect_cylindre(t_object *obj, t_ray *ray)
{
	t_cylindre_utils all_inter;

	up_disk_cylindre(&all_inter, obj, ray);
	down_disk_cylindre(&all_inter, obj, ray);
}