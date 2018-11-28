/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 15:31:34 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/27 14:32:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	cursor_up(void)
{
	g_ctx.focus = g_ctx.focus > g_ctx.cols - 1
		? g_ctx.focus - g_ctx.cols
		: (g_ctx.rows * g_ctx.cols) - (g_ctx.cols - g_ctx.focus);
	g_ctx.focus -= g_ctx.focus >= g_ctx.argc ? g_ctx.cols : 0;
}

void	cursor_down(void)
{
	g_ctx.focus = g_ctx.focus + g_ctx.cols < g_ctx.argc
		? g_ctx.focus + g_ctx.cols
		: g_ctx.focus % g_ctx.cols;
}

void	cursor_left(void)
{
	g_ctx.focus = g_ctx.focus ? g_ctx.focus - 1 : g_ctx.argc - 1;
}

void	cursor_right(int selected)
{
	if (selected)
		g_ctx.selected[g_ctx.focus] = !g_ctx.selected[g_ctx.focus];
	if (!selected || g_ctx.selected[g_ctx.focus])
		g_ctx.focus = g_ctx.focus + 1 < g_ctx.argc ? g_ctx.focus + 1 : 0;
}

void	delete_opt(void)
{
	size_t		i;

	if (!g_ctx.argc)
		return ;
	i = (int)g_ctx.focus;
	g_ctx.argv[i] = NULL;
	while (i + 1 < g_ctx.argc)
	{
		g_ctx.argv[i] = g_ctx.argv[i + 1];
		g_ctx.selected[i] = g_ctx.selected[i + 1];
		g_ctx.lens[i] = g_ctx.lens[i + 1];
		i += 1;
	}
	g_ctx.selected[i] = 0;
	g_ctx.lens[i] = 0;
	g_ctx.argv[i] = NULL;
	if (g_ctx.argc == 1)
		ft_select_exit(1);
	if (g_ctx.focus == i)
		g_ctx.focus -= 1;
	g_ctx.argc -= 1;
}
