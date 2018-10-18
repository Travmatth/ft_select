/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:55:12 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/17 21:28:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
** sort time of last access.
*/

int		sort_access(void *first, void *second)
{
	t_dir	*f;
	t_dir	*s;
	char	*first_name;
	char	*second_name;
	int		sort;

	f = (t_dir*)first;
	s = (t_dir*)second;
	if (f->atime != s->atime)
		return ((int)(f->atime > s->atime));
	if (f->atime_nsec != s->atime_nsec)
		return ((int)(f->atime_nsec > s->atime_nsec));
	first_name = ((t_dir*)f)->full ? ((t_dir*)f)->full : ((t_dir*)f)->name;
	second_name = ((t_dir*)s)->full ? ((t_dir*)s)->full : ((t_dir*)s)->name;
	sort = ft_strcmp(first_name, second_name);
	if (sort <= 0)
		return (1);
	return (0);
}

/*
** sort time of last modification
*/

int		sort_time(void *first, void *second)
{
	t_dir	*f;
	t_dir	*s;
	char	*first_name;
	char	*second_name;
	int		sort;

	f = (t_dir*)first;
	s = (t_dir*)second;
	if (f->mtime != s->mtime)
		return ((int)(f->mtime > s->mtime));
	if (f->mtime_nsec != s->mtime_nsec)
		return ((int)(f->mtime_nsec > s->mtime_nsec));
	first_name = ((t_dir*)f)->full ? ((t_dir*)f)->full : ((t_dir*)f)->name;
	second_name = ((t_dir*)s)->full ? ((t_dir*)s)->full : ((t_dir*)s)->name;
	sort = ft_strcmp(first_name, second_name);
	if (sort <= 0)
		return (1);
	return (0);
}

/*
** sort nodes alphabetically
*/

int		sort_alpha(void *first, void *second)
{
	char	*f;
	char	*s;
	int		sort;

	f = ((t_dir*)first)->full
		? ((t_dir*)first)->full
		: ((t_dir*)first)->name;
	s = ((t_dir*)second)->full
		? ((t_dir*)second)->full
		: ((t_dir*)second)->name;
	sort = ft_strcmp(f, s);
	if (sort <= 0)
		return (1);
	return (0);
}

/*
** except for . and .. directory entries, do not sort
*/

int		sort_null(void *first, void *second)
{
	char	*f;
	char	*s;

	f = ((t_dir*)first)->name;
	s = ((t_dir*)second)->name;
	if (ft_strequ(".", s))
		return (0);
	else if (ft_strequ("..", s) && !ft_strequ(".", f))
		return (0);
	return (1);
}

/*
** given a linked list of sorted nodes, swap orientation
** of nodes so that columns are sorted instead of rows
*/

t_list	*sort_line(t_list **nodes, unsigned short cols)
{
	t_list	**sorted;
	int		rows;
	int		size;
	int		i;

	size = (int)ft_lstsize(*nodes);
	rows = (size ? (int)size : 1) / (int)cols;
	rows = rows * cols < size ? rows + 1 : rows;
	if (!rows || rows == 1)
		return (*nodes);
	sorted = (t_list**)ft_memalloc(sizeof(t_list*) * rows);
	i = 0;
	while (i < size)
		ft_lstpushfront(&sorted[i++ % rows], ft_lsttail(nodes));
	i = rows;
	*nodes = NULL;
	while (--i >= 0)
	{
		if (ft_lstsize(sorted[i]) < cols)
			ft_lstpushfront(&sorted[i], ft_lstnew(NULL, 0));
		ft_lstpushback(nodes, sorted[i]);
	}
	free(sorted);
	return (*nodes);
}
