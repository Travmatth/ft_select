/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 15:31:34 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/13 16:27:32 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	cursor_up(int argc, char **argv, t_ctx *ctx)
{
	int		i;

	(void)argc;
	(void)argv;
	i = ctx->focus - ctx->cols != -1 ? ctx->focus - ctx->cols : ctx->rows - 1;
	ctx->focus = i;
}

void	cursor_down(int argc, char **argv, t_ctx *ctx)
{
	int		i;

	(void)argv;
	i = ctx->focus + ctx->cols < argc ? ctx->focus + ctx->cols : 0;
	ctx->focus = i;
}

void	cursor_left(int argc, char **argv, t_ctx *ctx)
{
	int		i;

	(void)argv;
	i = ctx->focus - 1 != -1 ? ctx->focus - 1 : argc - 1;
	ctx->focus = i;
}

void	cursor_right(int argc, char **argv, t_ctx *ctx, int selected)
{
	int		i;

	(void)argv;
	if (selected)
		ctx->selected[ctx->focus] = !ctx->selected[ctx->focus];
	if (!selected || ctx->selected[ctx->focus])
	{
		i = ctx->focus + 1 < argc ? ctx->focus + 1 : 0;
		ctx->focus = i;
	}
}
