/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/05 16:20:31 by tmatthew         ###   ########.fr       */
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

char	*format_args(int argc, char **argv, t_offset *offsets)
{
	unsigned short	i;
	size_t			max;
	size_t			current;
	char			*term;
	char			*template;

	i = 0;
	current = 0;
	max = get_term_size(argc, argv, offsets);
	offsets->width = max;
	if (!(term = (char*)ft_memalloc((((max + 1) * argc) + offsets->rows) * sizeof(char)))
		|| !(template = (char*)ft_memalloc((max + 1) * sizeof(char))))
		return (NULL);
	while (i < argc)
	{
		if (i == 0 && ++i)
			continue ;
		ft_snprintf(template, max + 1, "%- *s", (int)(max - LEN(argv[i], 0)), argv[i]);
		ft_memcpy(term + current, template, max);
		term[(current + max)] = ' ';
		current += max + 1;
		if (i % offsets->cols == offsets->cols - 1)
			term[current++ + 1] = '\n';
		i += 1;
	}
	free(template);
	return (term);
}

void	display(char *term, t_offset *offsets)
{
	(void)offsets;
	ft_putendl(term);
}

int		main(int argc, char **argv)
{
	t_offset	offsets;
	char		*term;

	ft_bzero(&offsets, sizeof(offsets));
	term = format_args(argc, argv, &offsets);
	display(term, &offsets);
	return (0);
}
