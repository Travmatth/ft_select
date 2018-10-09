/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 15:16:48 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/08 10:47:20 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int		find_hidden(t_list *elem)
{
	t_dir	*d;

	d = (t_dir*)elem->content;
	return (d->name && (d->name[0] == '.'));
}

void	free_dir(t_dir *dir)
{
	(void)dir;
}

void	remove_hidden(t_list *elem)
{
	free_dir((t_dir*)elem->content);
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
