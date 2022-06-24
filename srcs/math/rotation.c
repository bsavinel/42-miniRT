/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 17:35:50 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/23 10:01:20 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <math.h>

void	rotate_z(t_vec *point, double rotation)
{
	double	xtmp;
	double	ytmp;

	xtmp = point->x;
	ytmp = point->y;
	point->x = xtmp * cos(rotation) - ytmp * sin(rotation);
	point->y = xtmp * sin(rotation) + ytmp * cos(rotation);
}

void	rotate_y(t_vec *point, double rotation)
{
	double	xtmp;
	double	ztmp;

	xtmp = point->x;
	ztmp = point->z;
	point->x = xtmp * cos(rotation) + ztmp * sin(rotation);
	point->z = -xtmp * sin(rotation) + ztmp * cos(rotation);
}

void	rotate_x(t_vec *point, double rotation)
{
	double	ytmp;
	double	ztmp;

	ytmp = point->y;
	ztmp = point->z;
	point->y = ytmp * cos(rotation) - ztmp * sin(rotation);
	point->z = ytmp * sin(rotation) + ztmp * cos(rotation);
}