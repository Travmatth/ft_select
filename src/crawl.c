/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crawl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:56:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/02 21:06:59 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ft_ls.h"

void	harvest_node(t_dir *dir, t_dir *node, struct stat *attribs, int lens)
{
	size_t			len;
	struct passwd	*pw;
	struct group	*gr;

	node->dir = S_ISDIR(attribs->st_mode);
	node->links = ft_itoa(attribs->st_nlink);
	node->mode = attribs->st_mode;
	if ((pw = getpwuid(attribs->st_uid)))
		node->owner_name = pw->pw_name;
	if ((gr = getgrgid(attribs->st_gid)))
		node->owner_group = gr->gr_name;
	node->size = ft_itoa(attribs->st_size);
	node->date = ctime(&attribs->st_mtimespec.tv_sec);
	node->mtime = attribs->st_mtimespec.tv_nsec;
	node->atime = attribs->st_atimespec.tv_nsec;
	if (lens)
	{
		if ((len = LEN(node->links, 0)) > dir->width.link)
			dir->width.link = len;
		if ((len = LEN(node->size, 0)) > dir->width.size)
			dir->width.size = len;
		if ((len = LEN(node->owner_name, 0)) > dir->width.owner_name)
			dir->width.owner_name = len;
		if ((len = LEN(node->owner_name, 0)) > dir->width.owner_name)
			dir->width.owner_name = len;
	}
}

/*
** iterate through the given directories and add their subdirs to stack,
** add files to file linked list
*/

int		add_dir_name(char *parent, char *current, t_dir *node)
{
	char	*tmp;
	if (node->dir && !ft_strequ(".", current) && !ft_strequ("..", current))
	{
		tmp = ft_strjoin(parent, "/");
		node->name = ft_strjoin(tmp, current);
		free(tmp);
		return (1);
	}
	else
		node->name = current;
	return (0);
}

void	harvest_dir(t_ls *ctx, t_dir *dir)
{
	t_list			*dirs;
	t_dir			node;
	struct dirent	*d;
	DIR				*dr;
	struct stat		attribs;

	dirs = NULL;
	if ((dr = opendir(dir->name)))
	{
		while ((d = readdir(dr)))
		{
			lstat(d->d_name, &attribs);
			harvest_node(dir, &node, &attribs, 1);
			(add_dir_name(dir->name, d->d_name, &node))
				? ft_lstpushback(&dirs, ft_lstnew((void*)&node, sizeof(t_dir)))
				: ft_lstpushback(&dir->files, ft_lstnew((void*)&node
					, sizeof(t_dir)));
		}
		closedir(dr);
	}
	 dirs = ft_lstmergesort(ctx->compare
		, dirs, !GET_REVERSE(ctx->flags), ft_lstsize(dirs));
	 dir->files = ft_lstmergesort(ctx->compare
		, dir->files, !GET_REVERSE(ctx->flags), ft_lstsize(dir->files));
	ft_lstmerge(&ctx->stack, dirs);
}

/*
** traverse given directories  and subdirectories, printing at each stage
*/

void	crawl_files(t_ls *ctx)
{
	int		rev;
	t_list	*dirs;
	t_list	*d;

	rev = !GET_REVERSE(ctx->flags);
	ctx->compare = sort_null;
	GET_SORT_ACCESS(ctx->flags) ? (ctx->compare = sort_access) : NULL;
	GET_SORT_TIME(ctx->flags) ? (ctx->compare = sort_time) : NULL;
	dirs = ft_lstmergesort(ctx->compare, ctx->stack, rev, ft_lstsize(ctx->stack));
	while (dirs)
	{
		d = ft_lsttail(&dirs);
		harvest_dir(ctx, (t_dir*)d->content);
	}
}
