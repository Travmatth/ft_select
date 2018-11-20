/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 19:01:29 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/19 16:55:58 by tmatthew         ###   ########.fr       */
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

size_t	get_term_size(void)
{
	struct winsize	w;
	size_t			*max;
	size_t			total;
	size_t			col_width;

	ioctl(g_fd, TIOCGWINSZ, &w);
	max = (size_t*)ft_arrfoldl(parse_arg
		, g_ctx.argc, sizeof(char*), g_ctx.argv);
	if (g_ctx.argc == 0 || !w.ws_col || !w.ws_row || (size_t)w.ws_col < *max)
	{
		g_ctx.cols = 0;
		g_ctx.rows = 0;
		return (0);
	}
	g_ctx.win_col = (int)w.ws_col;
	g_ctx.win_row = (int)w.ws_row;
	col_width = *max + 1 > (size_t)g_ctx.cols / g_ctx.argc;
	col_width = col_width ? *max + 1 : (size_t)g_ctx.win_col / g_ctx.argc;
	g_ctx.cols = g_ctx.win_col / col_width;
	g_ctx.rows = (g_ctx.argc / g_ctx.cols) + 1;
	g_ctx.rows = g_ctx.rows ? g_ctx.rows : 1;
	total = *max;
	free(max);
	return (col_width);
}

void	format_args(void)
{
	int		i;
	size_t	current;

	i = -1;
	current = 0;
	if (!(g_ctx.lens = (size_t*)ft_memalloc(g_ctx.argc * sizeof(size_t)))
		|| !(g_ctx.selected = (short*)ft_memalloc(g_ctx.argc * sizeof(short))))
		return ;
	while (++i < g_ctx.argc)
		g_ctx.lens[i] = LEN(g_ctx.argv[i], 0);
}
