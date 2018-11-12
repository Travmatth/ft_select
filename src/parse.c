/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 19:01:29 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/11 17:53:50 by tmatthew         ###   ########.fr       */
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

size_t	get_term_size(int fd, int argc, char **argv, t_offset *offsets)
{
	struct winsize	w;
	size_t			*max;
	size_t			total;
	size_t			col_width;

	if (argc == 0)
	{
		offsets->cols = 0;
		offsets->rows = 0;
		return 0;
	}
	ioctl(fd, TIOCGWINSZ, &w);
	max = (size_t*)ft_arrfoldl(parse_arg, argc, sizeof(char*), argv);
	offsets->cols = (int)(w.ws_col ? w.ws_col : 80);
	col_width = *max + 1 > (size_t)offsets->cols / argc;
	col_width = col_width ? *max + 1 : (size_t)offsets->cols / argc;
	offsets->cols /= col_width;
	offsets->rows = argc / offsets->cols;
	offsets->rows = offsets->rows ? offsets->rows : 1;
	total = *max;
	free(max);
	return (col_width);
}

void	format_args(int fd, int argc, char **argv, t_offset *offsets)
{
	int		i;
	size_t	current;

	i = -1;
	current = 0;
	if (!(offsets->lens = (size_t*)ft_memalloc(argc * sizeof(size_t)))
		|| !(offsets->selected = (short*)ft_memalloc(argc * sizeof(short))))
		return ;
	offsets->width = get_term_size(fd, argc, argv, offsets);
	while (++i < argc)
		offsets->lens[i] = LEN(argv[i], 0);
}
