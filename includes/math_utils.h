/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:44:52 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/20 17:56:45 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H
# include "minirt_struct.h"

/*
 *	math_utils.c
 */

double	min(double a, double b);
double	max(double a, double b);

/*
 *	rotation.c
 */

void	rotate_x(t_vec *point, double rotation);
void	rotate_y(t_vec *point, double rotation);
void	rotate_z(t_vec *point, double rotation);

#endif
