/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 19:01:29 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/28 17:18:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	*parse_arg(void *final, void *elem, size_t i, int *stop)
{
	size_t	size;

	(void)i;
	if (!(final = final ? final : ft_memalloc(sizeof(size_t))))
	{
		*stop = 1;
		return (NULL);
	}
	size = LEN(*(char**)elem, 0);
	if (*(size_t*)final < size)
		*(size_t*)final = size;
	return (final);
}

size_t	get_term_size(int fd, t_ctx *ctx)
{
	struct winsize	w;
	size_t			*max;
	size_t			total;
	size_t			col_width;

	ioctl(fd, TIOCGWINSZ, &w);
	max = (size_t*)ft_arrfoldl(parse_arg
		, ctx->argc, sizeof(char*), ctx->argv);
	if (ctx->argc == 0 || !w.ws_col || !w.ws_row || (size_t)w.ws_col < *max)
		return (0);
	ctx->win_col = (int)w.ws_col;
	ctx->win_row = (int)w.ws_row;
	if (!(col_width = *max + 1 > (size_t)ctx->cols / ctx->argc)
		|| !(col_width = col_width
			? *max + 1 : (size_t)ctx->win_col / ctx->argc))
		return (0);
	if (!(ctx->cols = ctx->win_col / col_width))
		return (0);
	ctx->rows = (ctx->argc / ctx->cols) + 1;
	ctx->rows = ctx->rows ? ctx->rows : 1;
	total = *max;
	free(max);
	return (col_width);
}

void	format_args(t_ctx *ctx)
{
	size_t	i;
	size_t	current;

	i = 0;
	current = 0;
	if (!(ctx->lens = (size_t*)ft_memalloc(ctx->argc * sizeof(size_t)))
		|| !(ctx->selected = (short*)ft_memalloc(ctx->argc * sizeof(short))))
		return ;
	while (i < ctx->argc)
	{
		ctx->lens[i] = LEN(ctx->argv[i], 0);
		i += 1;
	}
}
