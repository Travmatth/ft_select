/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 15:16:48 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/11 16:31:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int		find_hidden(t_list *elem)
{
	t_dir	*d;

	d = (t_dir*)elem->content;
	if (d->name[0] == '.' && !d->name[1])
		return (1);
	if (d->name[0] == '.' && d->name[1] == '.')
		return (1);
	if (d->name[0] == '.' && d->name[1] != '/')
		return (1);
	return (0);
}

void	free_dir(void *d, size_t len)
{
	t_dir	*dir;

	(void)len;
	dir = (t_dir*)d;
	free(dir->name);
	free(dir->parent);
	free(dir->full);
	if (dir->links)
		free(dir->links);
	if (dir->owner_name)
		free(dir->owner_name);
	if (dir->owner_group)
		free(dir->owner_group);
	if (dir->size)
		free(dir->size);
	if (dir->total_out)
		free(dir->total_out);
	if (dir->date_str)
		free(dir->date_str);
	if (dir->name_str)
		free(dir->name_str);
	if (dir->format_str)
		free(dir->format_str);
	if (dir->files)
		ft_lstdel(&dir->files, free_dir);
	free(d);
}

int		find_files(t_list *elem)
{
	t_dir	*dir;

	dir = (t_dir*)elem->content;
	return (!dir->dir);
}

void	*get_max_width(void *final, t_list *elem, size_t i, int *stop)
{
	(void)i;
	(void)stop;
	t_dir	*dir;
	size_t	size;

	dir = (t_dir*)elem->content;
	size = LEN(dir->name, 0);
	if (!final)
		return (ft_memdup(&size, sizeof(size_t)));
	else if (*(size_t*)final < size)
	{
		free(final);
		return (ft_memdup(&size, sizeof(size_t)));
	}
	return (final);
}
