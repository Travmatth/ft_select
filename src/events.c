/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 15:31:34 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/15 14:03:36 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	cursor_up(int argc, char **argv, t_ctx *ctx)
{
	(void)argv;
	ctx->focus = ctx->focus > ctx->cols - 1
		? ctx->focus - ctx->cols
		: (ctx->rows * ctx->cols) - (ctx->cols - ctx->focus);
	ctx->focus -= ctx->focus >= (size_t)argc ? ctx->cols : 0;
}

void	cursor_down(int argc, char **argv, t_ctx *ctx)
{
	(void)argv;
	ctx->focus = ctx->focus + ctx->cols < (size_t)argc
		? ctx->focus + ctx->cols
		: ctx->focus % ctx->cols;
}

void	cursor_left(int argc, char **argv, t_ctx *ctx)
{
	(void)argv;
	ctx->focus = ctx->focus ? ctx->focus - 1 : (size_t)argc - 1;
}

void	cursor_right(int argc, char **argv, t_ctx *ctx, int selected)
{
	(void)argv;
	if (selected)
		ctx->selected[ctx->focus] = !ctx->selected[ctx->focus];
	if (!selected || ctx->selected[ctx->focus])
		ctx->focus = ctx->focus + 1 < (size_t)argc ? ctx->focus + 1 : 0;
}
