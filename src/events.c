/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 15:31:34 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/28 16:20:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	cursor_up(t_ctx *ctx)
{
	ctx->focus = ctx->focus > ctx->cols - 1
		? ctx->focus - ctx->cols
		: (ctx->rows * ctx->cols) - (ctx->cols - ctx->focus);
	ctx->focus -= ctx->focus >= ctx->argc ? ctx->cols : 0;
}

void	cursor_down(t_ctx *ctx)
{
	ctx->focus = ctx->focus + ctx->cols < ctx->argc
		? ctx->focus + ctx->cols
		: ctx->focus % ctx->cols;
}

void	cursor_left(t_ctx *ctx)
{
	ctx->focus = ctx->focus ? ctx->focus - 1 : ctx->argc - 1;
}

void	cursor_right(t_ctx *ctx, int selected)
{
	if (selected)
		ctx->selected[ctx->focus] = !ctx->selected[ctx->focus];
	if (!selected || ctx->selected[ctx->focus])
		ctx->focus = ctx->focus + 1 < ctx->argc ? ctx->focus + 1 : 0;
}

void	delete_opt(t_ctx *ctx)
{
	size_t		i;

	if (!ctx->argc)
		return ;
	i = (int)ctx->focus;
	ctx->argv[i] = NULL;
	while (i + 1 < ctx->argc)
	{
		ctx->argv[i] = ctx->argv[i + 1];
		ctx->selected[i] = ctx->selected[i + 1];
		ctx->lens[i] = ctx->lens[i + 1];
		i += 1;
	}
	ctx->selected[i] = 0;
	ctx->lens[i] = 0;
	ctx->argv[i] = NULL;
	if (ctx->argc == 1)
		ft_select_exit(1);
	if (ctx->focus == i)
		ctx->focus -= 1;
	ctx->argc -= 1;
}
