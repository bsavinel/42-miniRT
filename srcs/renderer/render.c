/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:52:40 by plouvel           #+#    #+#             */
/*   Updated: 2022/08/09 15:41:50 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "matrix.h"
#include "define.h"
#include "mlx.h"
#include "mlx_utils.h"
#include "tuple.h"
#include "texture.h"
#include "scene.h"
#include <math.h>
#include "math_utils.h"
#include "multithreading.h"
#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>

static inline uint32_t	get_color(t_color color)
{
	unsigned char r, g, b;
	double scale;

	scale = 1. / SAMPLE_PPX;
	color.x *= scale;
	color.y *= scale;
	color.z *= scale;
	r = min(255., min(1, color.x) * 255.99);
	g = min(255., min(1, color.y) * 255.99);
	b = min(255., min(1, color.z) * 255.99);
	return (r << 16 | g << 8 | b);
}

void	*render(void *pwrkrs)
{
	t_worker	*wrkr;
	t_ray		ray;
	t_rayhit	rayhit;
	t_object	*obj;
	t_point2	rp;
	t_color		color;

	wrkr = pwrkrs;
	rp.y = wrkr->assigned_start;

	while (rp.y <= wrkr->assigned_end)
	{
		rp.x = 0;
		while (rp.x < WIDTH)
		{
			color.x = 0;
			color.y = 0;
			color.z = 0;
			for (size_t i = 0; i < SAMPLE_PPX; i++)
			{
				generate_ray(&wrkr->minirt->camera, &ray, &rayhit, &rp);
				obj = ray_intersect_scene_objs(&wrkr->minirt->scene, &ray, &rayhit);
				if (obj)
					color = tadd(color, get_shade(&wrkr->minirt->scene, obj, &rayhit));
			}
			mlx_pixel_img_put(wrkr->minirt, rp.x, rp.y, get_color(color));
			rp.x++;
		}
		rp.y++;
	}
	return (NULL);
}

uint64_t	get_mlsec_time(struct timeval t)
{
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	render_img(t_minirt *minirt)
{
	struct timeval	t, t1;
	gettimeofday(&t, NULL);

	launch_workers(minirt);

	gettimeofday(&t1, NULL);
	printf("Rendered in %lu mlsec.\n", get_mlsec_time(t1) - get_mlsec_time(t));
	mlx_put_image_to_window(minirt->mlx.ptr, minirt->mlx.win, minirt->mlx.img.img, 0, 0);
}
