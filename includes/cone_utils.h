/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 11:40:25 by bsavinel          #+#    #+#             */
/*   Updated: 2022/08/09 11:41:49 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONE_UTILS_H
# define CONE_UTILS_H

#include "minirt_struct.h"

t_vec3	find_normal_cone(t_object *obj, t_rayhit rayhit);
void	feed_rayhit(t_rayhit *rayhit, t_object *obj, t_ray *true_ray, double t);

#endif