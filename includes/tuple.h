/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tuple.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:45:46 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/27 22:30:35 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TUPLE_H
# define TUPLE_H

# include "minirt_struct.h"

t_tuple		tuple(double x, double y, double z, double w);
t_vec3		vector(double x, double y, double z);
t_point3	point(double x, double y, double z);

t_tuple		tmul(t_tuple a, t_tuple b);
t_tuple		tmul_scalar(t_tuple a, double s);
t_tuple		tadd(t_tuple a, t_tuple b);
t_tuple		tsub(t_tuple a, t_tuple b);
t_tuple		tnegate(t_tuple a);
void		tprint(t_tuple a);

double		vec_dot(t_vec3 a, t_vec3 b);
double		vec_mag(t_vec3 a);
t_vec3		vec_norm(t_vec3 a);
double		vec_mag_sqr(t_vec3 a);
t_vec3	vec_cross(t_vec3 a, t_vec3 b);

#endif
