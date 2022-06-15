/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 10:25:45 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/15 14:39:32 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLANE_H
# define PLANE_H

# include "minirt_struct.h"

/*
 *	intersec_plane.c
 */

t_rayhit	plane_intersection(t_3dpoint point, t_vec3d normal, t_ray ray);

#endif