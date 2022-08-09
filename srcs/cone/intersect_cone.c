/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 17:09:25 by bsavinel          #+#    #+#             */
/*   Updated: 2022/08/09 10:59:45 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_utils.h"
#include "minirt_struct.h"
#include "ray_intersection.h"
#include "matrix.h"
#include "tuple.h"
#include <math.h>
#include "define.h"

bool	give_intersect_cone(t_ray *ray, double distance[2])
{
	double	a;
	double	b;
	double	c;

	a = ray->dir.x * ray->dir.x - ray->dir.y * ray->dir.y + ray->dir.z * ray->dir.z;
	b = (2 * ray->org.x * ray->dir.x) - (2 * ray->org.y * ray->dir.y) + (2 * ray->org.z * ray->dir.z);
	c = ray->org.x * ray->org.x - ray->org.y * ray->org.y + ray->org.z * ray->org.z;
	return (solve_quadratic(a, b, c, distance));
}

t_vec3	find_normal_cone(t_object *obj, t_rayhit rayhit)
{
	t_vec3	normal;
	double	y;

	y = sqrt(rayhit.intersect_p_local.x * rayhit.intersect_p_local.x + rayhit.intersect_p_local.z * rayhit.intersect_p_local.z);
	if (rayhit.intersect_p_local.y > 0)
		y = -y;
	normal = vector(rayhit.intersect_p_local.x, y, rayhit.intersect_p_local.z);
	return (vec_norm(matrix4_tmul(obj->M_inv_trans, normal)));
}

void	feed_rayhit(t_rayhit *rayhit, t_object *obj, t_ray *true_ray, double t)
{
	rayhit->t = t;
	rayhit->normal = find_normal_cone(obj, *rayhit);
	rayhit->intersect_p = get_ray_point(*true_ray, t);
}

void	intersect_cone_body(t_cone_utils *utils, t_object *obj, t_ray *ray, t_ray *true_ray)
{
	double		distance[2];

	if (give_intersect_cone(ray, distance))
	{
		utils->rayhit_first.intersect_p_local = get_ray_point(*ray, distance[0]);
		if (distance[0] >= 0 && utils->rayhit_first.intersect_p_local.y < 0 && utils->rayhit_first.intersect_p_local.y > -obj->p.cone.height)
		{
			utils->first = true;
			feed_rayhit(&utils->rayhit_first, obj, true_ray, distance[0]);
		}
		else if (distance[1] >= 0)
		{
			utils->rayhit_second.intersect_p_local = get_ray_point(*ray, distance[1]);
			if (utils->rayhit_second.intersect_p_local.y < 0 && utils->rayhit_second.intersect_p_local.y > -obj->p.cone.height)
			{
				utils->second = true;
				feed_rayhit(&utils->rayhit_second, obj, true_ray, distance[1]);
			}
		}
	}
}

void	base_cone(t_cone_utils *utils, t_object *obj, t_ray *ray, t_ray *true_ray)
{
	double	t;
	double	x;
	double	z;

	t = (-obj->p.cone.height - ray->org.y) / ray->dir.y;
	x = ray->org.x + ray->dir.x * t;
	z = ray->org.z + ray->dir.z * t;
	if (t > 0 && x * x + z * z <= 1)
	{
		utils->close_disk = true;
		utils->rayhit_close_disk.t = t;
		utils->rayhit_close_disk.normal = vector(0, -1, 0);
		utils->rayhit_close_disk.intersect_p = get_ray_point(*true_ray, t);
	}
}

bool	first_inter_cone(t_cone_utils *utils, t_rayhit *rayhit)
{
	bool		retour;

	retour = false;
	if (utils->first)
	{
		retour = true;
		*rayhit = utils->rayhit_first;
	}
	if (utils->second && (retour == false
			|| rayhit->t > utils->rayhit_second.t))
	{
		retour = true;
		*rayhit = utils->rayhit_second;
	}
	if (utils->close_disk && (retour == false
			|| rayhit->t > utils->rayhit_close_disk.t))
	{
		retour = true;
		*rayhit = utils->rayhit_close_disk;
	}
	return (retour);
}

bool	intersect_cone(t_object *obj, t_ray *ray, t_rayhit *rayhit)
{
	t_ray			new_ray;
	t_cone_utils	utils;

	utils.close_disk = false;
	utils.first = false;
	utils.second = false;
	new_ray = ray_transform(*ray, obj->M_inv);
	intersect_cone_body(&utils, obj, &new_ray, ray);
	base_cone(&utils, obj, &new_ray, ray);
	return (first_inter_cone(&utils, rayhit));
}
