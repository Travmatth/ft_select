/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 19:01:29 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/13 17:53:41 by tmatthew         ###   ########.fr       */
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

size_t	get_term_size(int argc, char **argv, t_ctx *ctx)
{
	struct winsize	w;
	size_t			*max;
	size_t			total;
	size_t			col_width;

	if (argc == 0)
	{
		ctx->cols = 0;
		ctx->rows = 0;
		return 0;
	}
	ioctl(g_fd, TIOCGWINSZ, &w);
	max = (size_t*)ft_arrfoldl(parse_arg, argc, sizeof(char*), argv);
	ctx->cols = (int)(w.ws_col ? w.ws_col : 80);
	col_width = *max + 1 > (size_t)ctx->cols / argc;
	col_width = col_width ? *max + 1 : (size_t)ctx->cols / argc;
	ctx->cols /= col_width;
	ctx->rows = argc / ctx->cols;
	ctx->rows = ctx->rows ? ctx->rows : 1;
	total = *max;
	free(max);
	return (col_width);
}

void	format_args(int argc, char **argv, t_ctx *ctx)
{
	int		i;
	size_t	current;

	i = -1;
	current = 0;
	if (!(ctx->lens = (size_t*)ft_memalloc(argc * sizeof(size_t)))
		|| !(ctx->selected = (short*)ft_memalloc(argc * sizeof(short))))
		return ;
	while (++i < argc)
		ctx->lens[i] = LEN(argv[i], 0);
}
