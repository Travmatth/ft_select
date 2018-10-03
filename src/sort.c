/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:55:12 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/02 20:24:57 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
** stat.ST_ATIME | Time of last access.
*/

int		sort_access(void *first, void *second)
{
	t_dir *f;
	t_dir *s;

	f = (t_dir*)first;
	s = (t_dir*)second;
	return (f->atime - s->atime);
}

/*
** stat.ST_MTIME | Time of last modification.
*/

/*
** stat.ST_CTIME | On Unix systems is the time of the last metadata change
*/

int		sort_time(void *first, void *second)
{
	t_dir *f;
	t_dir *s;

	f = (t_dir*)first;
	s = (t_dir*)second;
	return (f->mtime - s->mtime);
}

int		sort_null(void *first, void *second)
{
	(void)first;
	(void)second;
	return (1);
}
