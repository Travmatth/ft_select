/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crawl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:56:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/11 17:07:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ft_ls.h"

/*
** populate root dir struct with appropriate members
*/

void	set_root_dir(t_ls *ctx, t_dir *dir, char *dirname, char *files)
{
	*files = 1;
	ctx->top_lvl_dirs += 1;
	ft_bzero((void*)dir, sizeof(t_dir));
	dir->name = ft_strdup(dirname);
	dir->parent = ft_strdup(dirname);
	dir->full = dirname;
	dir->root = 1;
	dir->dir = GET_NO_RECURSE(ctx->flags) ? 0 : 1;
}

/*
** populate given node with appropriate attribute members
*/

void	harvest_node(t_ls *ctx, t_dir *node, struct stat *attribs)
{
	struct passwd	*pw;
	struct group	*gr;

	node->dir = S_ISDIR(attribs->st_mode) && !GET_NO_RECURSE(ctx->flags) ? 1 : 0;
	node->mtime = attribs->st_mtimespec.tv_sec;
	node->mtime_nsec = attribs->st_mtimespec.tv_nsec;
	node->atime = attribs->st_atimespec.tv_sec;
	node->atime_nsec = attribs->st_atimespec.tv_nsec;
	node->mode = attribs->st_mode;
	if (!GET_LONG(ctx->flags))
		return ;
	node->date = ctime(&attribs->st_mtimespec.tv_sec);
	node->links = ft_itoa(attribs->st_nlink);
	if ((pw = getpwuid(attribs->st_uid)))
		node->owner_name = ft_strdup(pw->pw_name);
	if ((gr = getgrgid(attribs->st_gid)))
		node->owner_group = ft_strdup(gr->gr_name);
	node->size = ft_itoa(attribs->st_size);
}

/*
** iterate through given directory, harvesting contents and adding output
** to appropriate stacks
*/

void	harvest_dir_nodes(t_ls *ctx, t_dir *dir, t_list **dirs, DIR *dr)
{
	struct dirent	*d;
	t_dir			node;
	struct stat		attribs;

	while ((d = readdir(dr)))
	{
		ft_bzero(&node, sizeof(t_dir));
		if (ERR(lstat((node.full = form_path(dir, d->d_name)), &attribs)))
		{
			if (errno == EACCES)
				dir->denied = 1;
			continue ;
		}
		add_to_total(ctx, d->d_name, dir, attribs.st_blocks);
		harvest_node(ctx, &node, &attribs);
		if (add_dir_name(dir, d->d_name, &node) && GET_RECURSE(ctx->flags))
		{
			ft_lstpushback(dirs, ft_lstnew((void*)&node, sizeof(t_dir)));
			deduplicate_node(ft_lstlast(*dirs));
		}
		ft_lstpushback(&dir->files, ft_lstnew((void*)&node, sizeof(t_dir)));
	}
}

/*
** harvest files of the given directory, adding files to dir->files
** and adding directories  onto both dir->files and the stack of directories
** to be harvested
*/

void	harvest_dir(t_ls *ctx, t_dir *dir)
{
	t_list			*dirs;
	DIR				*dr;

	dirs = NULL;
	errno = 0;
	if (dir->dir && (dr = opendir((dir->full = form_dir(dir, dir->name)))))
	{
		harvest_dir_nodes(ctx, dir, &dirs, dr);
		closedir(dr);
		dirs = ft_lstmergesort(ctx->compare
			, dirs, !GET_REVERSE(ctx->flags), ft_lstsize(dirs));
		dir->files = ft_lstmergesort(ctx->compare
			, dir->files, !GET_REVERSE(ctx->flags), ft_lstsize(dir->files));
		if (!GET_ALL(ctx->flags))
			dirs = ft_lstfilter(dirs, find_hidden, free_dir);
		ft_lstmerge(&ctx->stack, dirs);
	}
	else if (errno == EACCES)
		dir->denied = 1;
}

/*
** if given a stack of files to print, do so
** else if given a stack of directories to harvest, will sort
** and print the contents of each directory as it is harvested
*/

void	crawl_files(t_ls *ctx)
{
	int		rev;
	t_list	*d;
	t_list	*files;
	size_t	len;

	rev = !GET_REVERSE(ctx->flags);
	ctx->compare = sort_alpha;
	GET_NO_SORT(ctx->flags) ? (ctx->compare = sort_null) : NULL;
	GET_SORT_TIME(ctx->flags) ? (ctx->compare = sort_time) : NULL;
	GET_SORT_ACCESS(ctx->flags) && GET_SORT_TIME(ctx->flags) ? (ctx->compare = sort_access) : NULL;
	files = ft_lstseparate(&ctx->stack, find_files);
	len = ft_lstsize(files);
	files = ft_lstmergesort(ctx->compare, files, rev, len);
	len = ft_lstsize(ctx->stack);
	ctx->stack = ft_lstmergesort(ctx->compare, ctx->stack, rev, len);
	if (files)
		print_files(ctx, files);
	while (ctx->stack)
	{
		d = ft_lsttail(&ctx->stack);
		harvest_dir(ctx, (t_dir*)d->content);
		print_dir(ctx, (t_dir*)d->content);
		ft_lstdel(&d, free_dir);
	}
}
