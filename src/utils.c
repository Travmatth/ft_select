/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 15:16:48 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/12 18:14:52 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
** find whether given symbolic links underlying file is a directory
*/

void	set_dir_status(t_ls *ctx, t_dir *node, struct stat *attribs)
{
	char		buf[256];
	ssize_t		bytes;
	struct stat	orig_attribs;

	if (S_ISLNK(attribs->st_mode))
	{
		ft_bzero(buf, 256);
		bytes = readlink(node->full, buf, 255);
		lstat(buf, &orig_attribs);
		node->dir = S_ISDIR(orig_attribs.st_mode) ? 1 : 0;
		node->dir = GET_LONG(ctx->flags) ? 0 : node->dir;
		node->dir = GET_NO_RECURSE(ctx->flags) ? 0 : node->dir;
		return ;
	}
	node->dir = S_ISDIR(attribs->st_mode) ? 1 : 0;
	node->dir = GET_NO_RECURSE(ctx->flags) ? 0 : node->dir;
}

/*
** find hidden files
*/

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

/*
** free given directory node
*/

void	conditional_free(t_dir *dir)
{
	if (!dir->root)
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
}

/*
** free given directory node
*/

void	free_dir(void *d, size_t len)
{
	t_dir	*dir;

	(void)len;
	dir = (t_dir*)d;
	free(dir->name);
	free(dir->parent);
	conditional_free(dir);
	free(d);
}

/*
** find nodes with are plain files, not directories
*/

int		find_files(t_list *elem)
{
	t_dir	*dir;

	dir = (t_dir*)elem->content;
	return (!dir->dir);
}
