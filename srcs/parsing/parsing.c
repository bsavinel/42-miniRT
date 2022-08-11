/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 13:43:46 by bsavinel          #+#    #+#             */
/*   Updated: 2022/08/10 22:33:20 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "minirt_lexer.h"
#include "mlx_utils.h"
#include "scene.h"
#include "libft.h"
#include "tuple.h"
#include "parsing.h"
#include "define.h"
#include <stdlib.h>

bool ft_isdouble(char *str)
{
	int	i;
	int	point;
	
	i = 0;
	point = 0;
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			if (point == 1)
				return false;
			if (str[i] == '.')
				point++;
			else
				return false;
		}
		i++;
	}
	return (true);
}

void	bad_exit_msg(t_minirt *minirt, char *str)
{
	ft_lstclear(&minirt->start_lexer, free_token);
	ft_lstclear(&minirt->scene.light, &free);
	ft_lstclear(&minirt->scene.objs, &free);
	destruct_mlx(&minirt->mlx);
	printf("Error\n%s\n", str);
	exit(1);
}

void	select_object(t_minirt *minirt, t_list **lexer)
{
	if (D_LEX_CONTENT->type == T_SPHERE)
		extract_sphere(minirt, lexer);
	else if (D_LEX_CONTENT->type == T_PLAN)
		extract_plan(minirt, lexer);
	else if (D_LEX_CONTENT->type == T_CYLINDER)
		extract_cylinder(minirt, lexer);
	else if (D_LEX_CONTENT->type == T_CAMERA)
		extract_camera(minirt, lexer);
	else if (D_LEX_CONTENT->type == T_LIGHT)
		extract_light(minirt, lexer);
	else if (D_LEX_CONTENT->type == T_AMBIANT_LIGHT)
		extract_ambiante_light(minirt, lexer);
	else if (D_BONUS && D_LEX_CONTENT->type == T_CONE)
		extract_cone(minirt, lexer);
}

bool	feed_scene(t_minirt *minirt, t_list **lexer)
{
	bool	new_line;
	t_list	*save_lexer;

	while (*lexer && D_LEX_CONTENT->type != T_NULL)
	{
		new_line = false;
		save_lexer = *lexer;
		select_object(minirt, lexer);
		if (*lexer && D_LEX_CONTENT->type == T_NEWLINE)
		{
			new_line = true;
			*lexer = (*lexer)->next;
		}
		if (*lexer && ((new_line == false && D_LEX_CONTENT->type != T_NULL)
				|| save_lexer == *lexer))
			bad_exit_msg(minirt, "Patern not identify");
	}
	return (true);
}

bool	parser(t_minirt *minirt, char *filename)
{
	t_list	*lex_file;

	lex_file = lex_from_file(filename);
	if (lex_file == NULL)
		bad_exit_msg(minirt, "lexing fail");
	minirt->start_lexer = lex_file;
	if (!feed_scene(minirt, &lex_file))
		bad_exit_msg(minirt, "Sommething bad arrive");
	return (true);
}