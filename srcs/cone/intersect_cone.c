/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 17:09:25 by bsavinel          #+#    #+#             */
/*   Updated: 2022/08/09 11:43:54 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_utils.h"
#include "minirt_struct.h"
#include "ray_intersection.h"
#include "matrix.h"
#include "tuple.h"
#include <math.h>
#include "cone_utils.h"
#include "define.h"

bool	give_intersect_cone(t_ray *ray, double distance[2])
{
	double	a;
	double	b;
	double	c;

	a = ray->dir.x * ray->dir.x - ray->dir.y * ray->dir.y
		+ ray->dir.z * ray->dir.z;
	b = (2 * ray->org.x * ray->dir.x) - (2 * ray->org.y * ray->dir.y)
		+ (2 * ray->org.z * ray->dir.z);
	c = ray->org.x * ray->org.x - ray->org.y * ray->org.y
		+ ray->org.z * ray->org.z;
	return (solve_quadratic(a, b, c, distance));
}

bool	intersect_cone_body(t_rayhit *rayhit, t_object *obj,
							t_ray *ray, t_ray *true_ray)
{
	double		distance[2];

	if (give_intersect_cone(ray, distance))
	{
		rayhit->intersect_p_local = get_ray_point(*ray, distance[0]);
		if (distance[0] >= 0 && rayhit->intersect_p_local.y < 0
			&& rayhit->intersect_p_local.y > -obj->p.cone.height)
		{
			feed_rayhit(rayhit, obj, true_ray, distance[0]);
			return (true);
		}
		else if (distance[1] >= 0)
		{
			rayhit->intersect_p_local = get_ray_point(*ray, distance[1]);
			if (rayhit->intersect_p_local.y < 0
				&& rayhit->intersect_p_local.y > -obj->p.cone.height)
			{
				feed_rayhit(rayhit, obj, true_ray, distance[1]);
				return (true);
			}
		}
	}
	return (false);
}

bool	base_cone(t_rayhit *rayhit, t_object *obj, t_ray *ray, t_ray *true_ray)
{
	double	t;
	double	x;
	double	z;

	t = (-obj->p.cone.height - ray->org.y) / ray->dir.y;
	x = ray->org.x + ray->dir.x * t;
	z = ray->org.z + ray->dir.z * t;
	if (t > 0 && x * x + z * z <= 1)
	{
		rayhit->t = t;
		rayhit->normal = vector(0, -1, 0);
		rayhit->intersect_p = get_ray_point(*true_ray, t);
		return (true);
	}
	return (false);
}

bool	first_inter_cone(t_cone_utils *utils, t_rayhit *rayhit)
{
	bool		retour;

	retour = false;
	if (utils->body)
	{
		retour = true;
		*rayhit = utils->rayhit_body;
	}
	if (utils->base && (retour == false
			|| rayhit->t > utils->rayhit_base.t))
	{
		retour = true;
		*rayhit = utils->rayhit_base;
	}
	return (retour);
}

bool	intersect_cone(t_object *obj, t_ray *ray, t_rayhit *rayhit)
{
	t_ray			new_ray;
	t_cone_utils	utils;

	new_ray = ray_transform(*ray, obj->M_inv);
	utils.body = intersect_cone_body(&utils.rayhit_body, obj, &new_ray, ray);
	utils.base = base_cone(&utils.rayhit_base, obj, &new_ray, ray);
	return (first_inter_cone(&utils, rayhit));
}
