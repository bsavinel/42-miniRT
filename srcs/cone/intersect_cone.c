/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 17:09:25 by bsavinel          #+#    #+#             */
/*   Updated: 2022/08/08 18:32:50 by bsavinel         ###   ########.fr       */
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

t_vec3	find_normal_cone(t_object *obj, t_rayhit rayhit, t_ray *ray)
{
	t_vec3	normal;
	double	y;
	

	//rayhit.intersect_p_local = point(1, 1, 1);
	
	y = sqrt(rayhit.intersect_p_local.x * rayhit.intersect_p_local.x + rayhit.intersect_p_local.z * rayhit.intersect_p_local.z);
	if (rayhit.intersect_p_local.y > 0)
		y = -y;
	normal = vector(rayhit.intersect_p_local.x, y, rayhit.intersect_p_local.z);
	

	/*printf("%lf, %lf, %lf\n", normal.x, normal.y, normal.z);
	exit(1);*/
	
	return (vec_norm(matrix4_tmul(obj->M_inv_trans, normal)));
}

void	intersect_cone_inf(t_cone_utils *utils, t_object *obj, t_ray *ray, t_ray *true_ray)
{
	double		distance[2];

	if (give_intersect_cone(ray, distance))
	{
		/*printf("%lf    %lf\n", distance[0],distance[1]);  //? test distance
		exit(1);*/
		if (distance[0] >= 0)
		{
			utils->rayhit_first.intersect_p_local = get_ray_point(*ray, distance[0]);
			utils->first = true;
			utils->rayhit_first.t = distance[0];
			utils->rayhit_first.normal = find_normal_cone(obj, utils->rayhit_first, ray);
			if (utils->rayhit_first.intersect_p_local.y > 0)
				utils->first = false;
			utils->rayhit_first.intersect_p = get_ray_point(*true_ray, distance[0]);
		}
		if (distance[1] >= 0)
		{
			utils->rayhit_second.intersect_p_local = get_ray_point(*ray, distance[1]);
			utils->second = true;
			utils->rayhit_second.t = distance[1];
			utils->rayhit_second.normal = find_normal_cone(obj, utils->rayhit_second, ray);
			if (utils->rayhit_second.intersect_p_local.y > 0)
				utils->second = false;
			utils->rayhit_second.intersect_p = get_ray_point(*true_ray, distance[1]);
		}
	}
}

bool	first_inter_cone(t_cone_utils *utils, t_object *obj, t_rayhit *rayhit)
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

/*void	cone_base(t_cone_utils *utils, t_object *obj, t_ray *ray)
{
	t_object	obj_tmp;

	obj_tmp.p.disk.center = tadd(obj->p.cone.top,
			tmul_scalar(obj->p.cone.dir, obj->p.cone.height));
	obj_tmp.p.disk.normal = obj->p.cone.dir;
	obj_tmp.p.disk.rayon = obj->p.cone.rayon_base;
	utils->close_disk = intersect_disk(&obj_tmp, ray);
	if (utils->close_disk)
		utils->rayhit_close_disk = obj_tmp.rayhit;
}*/

bool	intersect_cone(t_object *obj, t_ray *ray, t_rayhit *rayhit)
{
	t_ray			new_ray;
	t_cone_utils	utils;

	/*ray->dir = vec_norm(vector(-0.5,-1,1)); //? test valeur retour
	ray->org = point(1,1,-5);
	intersect_cone_inf(&utils, obj, ray, ray);*/
	utils.close_disk = false;
	utils.first = false;
	utils.second = false;
	new_ray = ray_transform(*ray, obj->M_inv);
	//cone_base(&utils, obj, &new_ray);
	intersect_cone_inf(&utils, obj, &new_ray, ray);
	
	return (first_inter_cone(&utils, obj, rayhit));
}