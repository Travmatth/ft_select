/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 19:01:29 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/08 18:23:45 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	*parse_arg(void *final, void *elem, size_t i, int *stop)
{
	size_t	size;

	if (i == 0)
		return (NULL);
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

size_t	get_term_size(int argc, char **argv, t_offset *offsets)
{
	struct winsize	w;
	size_t			*max;
	size_t			total;

	if (argc == 0)
	{
		offsets->cols = 0;
		offsets->rows = 0;
		return 0;
	}
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	max = (size_t*)ft_arrfoldl(parse_arg, argc, sizeof(char*), argv);
	offsets->cols = (int)(w.ws_col ? w.ws_col : 80 / (*max + 1));
	offsets->rows = argc / offsets->cols;
	offsets->rows = offsets->rows ? offsets->rows : 1;
	total = *max;
	free(max);
	return (total);
}

void	format_args(int argc, char **argv, t_offset *offsets)
{
	int		i;
	size_t	current;

	i = -1;
	current = 0;
	if (!(offsets->lens = (size_t*)ft_memalloc(argc * sizeof(size_t)))
		|| !(offsets->selected = (short*)ft_memalloc(argc * sizeof(short))))
		return ;
	offsets->width = get_term_size(argc, argv, offsets);
	while (++i < argc)
		offsets->lens[i] = LEN(argv[i], 0);
}
