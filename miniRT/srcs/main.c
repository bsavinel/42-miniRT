/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 17:59:46 by bsavinel          #+#    #+#             */
/*   Updated: 2022/08/11 17:06:49 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"
#include "define.h"
#include "parsing.h"
#include "end.h"
#include "renderer.h"
#include "multithreading.h"
#include "minirt_lexer.h"
#include "ft_dprintf.h"
#include "minirt_lexer.h"
#include <pthread.h>
#include <string.h>
#include "matrix.h"
#include "texture.h"
#include "scene.h"
#include "tuple.h"
#include "scene.h"
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "mlx.h"

int	main(int ac, char **av)
{
	t_minirt	minirt;

	ft_memset(&minirt, 0, sizeof(t_minirt));
	if (ac != 2)
	{
		if (*av)
			ft_dprintf(STDERR_FILENO, STR_BAD_NB_ARG, *av);
		else
			ft_dprintf(STDERR_FILENO, STR_BAD_NB_ARG, STR_PRG_NAME);
		return (1);
	}
	if (!init_mlx_struct(&minirt.mlx))
		return (1);
	printf("minirt: loading file '%s'...\n", av[1]);
	parser(&minirt, av[1]);
	printf("minirt: successfully loaded.\n");
	if (!init_mlx_window(&minirt))
		return (1);
	setup_workers(&minirt);
	render_img(&minirt);
	launch_loop(&minirt);
	return (0);
}
