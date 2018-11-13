/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 15:31:34 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/12 19:49:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	cursor_up(int argc, char **argv, t_offset *offsets)
{
	(void)argc;
	(void)argv;
	(void)offsets;
}

void	cursor_down(int argc, char **argv, t_offset *offsets)
{
	(void)argc;
	(void)argv;
	(void)offsets;
}

void	cursor_left(int argc, char **argv, t_offset *offsets)
{
	int		i;

	(void)argv;
	i = offsets->focus - 1 != -1 ? offsets->focus - 1 : argc - 1;
	offsets->focus = i;
}

void	cursor_right(int argc, char **argv, t_offset *offsets, int selected)
{
	int		i;

	(void)argv;
	if (selected)
		offsets->selected[offsets->focus] = !offsets->selected[offsets->focus];
	if (!selected || offsets->selected[offsets->focus])
	{
		i = offsets->focus + 1 < argc ? offsets->focus + 1 : 0;
		offsets->focus = i;
	}
}
