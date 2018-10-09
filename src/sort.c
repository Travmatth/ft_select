/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:55:12 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/08 11:59:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
** stat.ST_ATIME | Time of last access.
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
		return ((int)(f->atime >= s->atime));
	if (f->atime_nsec != s->atime_nsec)
		return ((int)(f->atime >= s->atime));
	first_name = ((t_dir*)f)->full ? ((t_dir*)f)->full : ((t_dir*)f)->name;
	second_name = ((t_dir*)s)->full ? ((t_dir*)s)->full : ((t_dir*)s)->name;
	sort = ft_strcmp(first_name, second_name);
	if (sort <= 0)
		return (1);
	return (0);
}

/*
** stat.ST_MTIME | Time of last modification.
*/

/*
** stat.ST_CTIME | On Unix systems is the time of the last metadata change
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
		return ((int)(f->mtime >= s->mtime));
	if (f->mtime_nsec != s->mtime_nsec)
		return ((int)(f->mtime >= s->mtime));
	first_name = ((t_dir*)f)->full ? ((t_dir*)f)->full : ((t_dir*)f)->name;
	second_name = ((t_dir*)s)->full ? ((t_dir*)s)->full : ((t_dir*)s)->name;
	sort = ft_strcmp(first_name, second_name);
	if (sort <= 0)
		return (1);
	return (0);
}

int		sort_alpha(void *first, void *second)
{
	char 	*f;
	char 	*s;
	int		sort;

	f = ((t_dir*)first)->full ? ((t_dir*)first)->full : ((t_dir*)first)->name;
	s = ((t_dir*)second)->full ? ((t_dir*)second)->full : ((t_dir*)second)->name;
	sort = ft_strcmp(f, s);
	if (sort <= 0)
		return (1);
	return (0);
}
