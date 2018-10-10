/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crawl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:56:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/10 00:49:30 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ft_ls.h"

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
** iterate through the given directories and add their subdirs to stack,
** add files to file linked list
*/

int		add_dir_name(t_dir *dir, char *current, t_dir *node)
{
	size_t			len;

	node->name = ft_strdup(current);
	if ((len = LEN(current, 0)) > dir->name_width)
		dir->name_width = len;
	node->parent = ft_strdup(dir->parent);
	if ((len = LEN(node->parent, 0)) > dir->parent_width)
		dir->parent_width = len;
	if (node->dir && !ft_strequ(".", current) && !ft_strequ("..", current))
		return (1);
	return (0);
}

char	*form_dir(t_dir *dir, char *name)
{
	size_t	len;
	char	*str;

	if (dir->root)
		return (ft_strdup(dir->name));
	if (dir->full)
		return (dir->full);
	len = LEN(dir->parent, 0) + LEN(name, 0) + 1;
	str = ft_strnew(len);
	ft_memcpy((void*)str, dir->parent, LEN(dir->parent, 0));
	ft_strcat(str, "/");
	ft_strcat(str, name);
	return (str);
}

char	*form_path(t_dir *dir, char *name)
{
	size_t	len;
	char	*str;

	len = LEN(dir->full, 0) + LEN(name, 0) + 1;
	str = ft_strnew(len);
	ft_memcpy((void*)str, dir->full, LEN(dir->full, 0));
	ft_strcat(str, "/");
	ft_strcat(str, name);
	return (str);
}

void	add_to_total(t_ls *ctx, char *name, t_dir *dir, int size)
{
	if (name[0] == '.' && GET_ALL(ctx->flags))
		dir->total += size;
	else if (name[0] != '.')
		dir->total += size;
}

void	deduplicate_node(t_list *node)
{
	t_dir *n;

	n = (t_dir*)node->content;
	n->name = ft_strdup(n->name);
	n->parent = ft_strdup(n->parent);
	n->full = ft_strdup(n->full);
	n->links = ft_strdup(n->links);
	n->owner_name = ft_strdup(n->owner_name);
	n->owner_group = ft_strdup(n->owner_group);
	n->size = ft_strdup(n->size);
	n->date = ft_strdup(n->date);
	n->total_out = ft_strdup(n->total_out);
	n->date_str = ft_strdup(n->date_str);
	n->name_str = ft_strdup(n->name_str);
	n->format_str = ft_strdup(n->format_str);
}

void	harvest_dir(t_ls *ctx, t_dir *dir)
{
	t_list			*dirs;
	t_dir			node;
	struct dirent	*d;
	DIR				*dr;
	struct stat		attribs;

	dirs = NULL;
	errno = 0;
	if (dir->dir && (dr = opendir((dir->full = form_dir(dir, dir->name)))))
	{
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
				ft_lstpushback(&dirs, ft_lstnew((void*)&node, sizeof(t_dir)));
				deduplicate_node(ft_lstlast(dirs));
			}
			ft_lstpushback(&dir->files, ft_lstnew((void*)&node, sizeof(t_dir)));
		}
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
** traverse given directories  and subdirectories, printing at each stage
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
