/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 11:10:25 by bsavinel          #+#    #+#             */
/*   Updated: 2022/08/07 14:48:31 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

# include <stddef.h> // -> NULL

# define THREAD_NBR 8

/*
 *	Define windows
 */

# define WIDTH 400
# define HEIGHT 400

# define EPSILON 0.0001

/*
 *	Temporary define
 */

# define FOV_ANGLE 90.0
# define FOV       (FOV_ANGLE * M_PI) / 180

# define ANGLE(x) (x * M_PI) / 180
/*
 *	Define hook
 */

# define D_ESCAPE 65307

/*
 *	Error message
 */

# define BAD_NB_ARG "Bad number of argument\nUse like this ./miniRT <file>\n"

#endif
