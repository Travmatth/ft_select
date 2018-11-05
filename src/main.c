/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/04 21:28:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	*parse_arg(void *final, void *elem, size_t i, int *stop)
{
	size_t	size;
	t_args	*args;

	if (i == 0)
		return (NULL);
	if (!(final = final ? final : ft_memalloc(sizeof(t_args))))
	{
		*stop = 1;
		return (NULL);
	}
	args = (t_args*)final;
	size = LEN(*(char**)elem, 0);
	if (!args->max)
		args->max = size;
	else if ((size_t)args->max < size)
		args->max = size;
	args->total += size;
	return (final);
}

size_t	get_term_size(int argc, char **argv, t_offset *offsets)
{
	struct winsize	w;
	t_args			*parsed_args;
	size_t			total;

	if (argc == 0)
	{
		offsets->cols = 0;
		offsets->rows = 0;
	}
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	parsed_args = (t_args*)ft_arrfoldl(parse_arg, argc, sizeof(char*), argv);
	offsets->cols = (int)(w.ws_col ? w.ws_col : 80 / (parsed_args->max + 1));
	offsets->rows = argc / offsets->cols;
	offsets->rows = offsets->rows ? offsets->rows : 1;
	total = parsed_args->total;
	free(parsed_args);
	return (total);
}

char	*display_args(int argc, char **argv, t_offset *offsets)
{
	unsigned short	i;
	size_t			total;
	size_t			current;
	char			*term;

	i = 0;
	current = 0;
	total = get_term_size(argc, argv, offsets);
	total += argc ? argc + (argc / offsets->rows) - 1 : 0;
	if (!(offsets->len = ft_memalloc((argc + 1) * sizeof(size_t)))
		|| !(offsets->start = ft_memalloc((argc + 1) * sizeof(size_t)))
		|| !(term = (char*)ft_memalloc(sizeof(char) * (total + 1))))
		return (NULL);
	while (i < argc)
	{
		offsets->len[i] = LEN(argv[i], 0);
		ft_memcpy(term + current, argv[i], offsets->len[i]);
		term[current + offsets->len[i]] = ' ';
		if (i++ % offsets->cols == offsets->cols - 1)
			term[current++ + offsets->len[i] + 1] = '\n';
		current += offsets->len[i] + 1;
	}
	return (term);
}

int		main(int argc, char **argv)
{
	t_offset	offsets;

	ft_bzero(&offsets, sizeof(offsets));
	display_args(argc, argv, &offsets);
	return (0);
}
