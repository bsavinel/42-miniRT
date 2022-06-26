/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 22:11:16 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/26 10:48:25 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"

/* uv_mapping.c */

t_uv	get_planar_map(t_point3 intersect_p);
t_uv	get_spherical_map(t_point3 intersect_p);

/* texture.c */

t_color		get_image_color(t_texture texture, t_uv uv);
t_texture	create_image_texture(void *mlx, const char *path);
t_texture	create_checkered_texture(int width, int height, uint32_t a,
		uint32_t b);
t_color	get_checker_color(t_texture texture, t_uv uv);
void	apply_obj_texture(t_object *obj, t_texture texture);
