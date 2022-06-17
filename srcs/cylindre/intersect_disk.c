/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_disk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:25:34 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/17 16:42:33 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "vector.h"

bool	intersect_disk(t_object *obj, t_ray *ray)
{
	obj->p.plan.point = obj->p.disk.center;
	obj->p.plan.normal = obj->p.disk.normal;
	if (plane_intersection(obj, ray) && vec_length(vec_sub(obj->p.disk.center,
				obj->rayhit.intersect_p)) <= obj->p.disk.rayon)
		return (true);
	return (false);
}

