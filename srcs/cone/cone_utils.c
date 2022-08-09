/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 11:42:18 by bsavinel          #+#    #+#             */
/*   Updated: 2022/08/09 12:35:13 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_utils.h"
#include "minirt_struct.h"
#include "matrix.h"
#include "tuple.h"
#include <math.h>

t_vec3	find_normal_cone(t_object *obj, t_rayhit rayhit)
{
	t_vec3	normal;
	double	y;

	y = sqrt(rayhit.intersect_p_local.x * rayhit.intersect_p_local.x
			+ rayhit.intersect_p_local.z * rayhit.intersect_p_local.z);
	normal = vector(rayhit.intersect_p_local.x, y, rayhit.intersect_p_local.z);
	normal = vec_norm(matrix4_tmul(obj->M_inv_trans, normal));
	normal.w = 0;
	return (normal);
}

void	feed_rayhit(t_rayhit *rayhit, t_object *obj, t_ray *true_ray, double t)
{
	rayhit->t = t;
	rayhit->normal = find_normal_cone(obj, *rayhit);
	rayhit->intersect_p = matrix4_tmul(obj->M, rayhit->intersect_p_local);
	//rayhit->intersect_p = get_ray_point(*true_ray, rayhit->t);
}
