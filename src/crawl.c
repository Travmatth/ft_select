/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crawl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:56:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/05 21:41:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ft_ls.h"

void	harvest_node(t_ls *ctx, t_dir *dir, t_dir *node, struct stat *attribs)
{
	size_t			len;
	struct passwd	*pw;
	struct group	*gr;

	node->dir = S_ISDIR(attribs->st_mode);
	if (!GET_LONG(ctx->flags))
		return ;
	node->mode = attribs->st_mode;
	node->date = ctime(&attribs->st_mtimespec.tv_sec);
	node->mtime = attribs->st_mtimespec.tv_nsec;
	node->atime = attribs->st_atimespec.tv_nsec;
	node->links = ft_itoa(attribs->st_nlink);
	if (dir && (len = LEN(node->links, 0)) > dir->width.link)
		dir->width.link = len;
	if ((pw = getpwuid(attribs->st_uid)))
		node->owner_name = pw->pw_name;
	if (dir && (len = LEN(node->owner_name, 0)) > dir->width.owner_name)
		dir->width.owner_name = len;
	if ((gr = getgrgid(attribs->st_gid)))
		node->owner_group = gr->gr_name;
	if (dir && (len = LEN(node->owner_group, 0)) > dir->width.owner_group)
		dir->width.owner_group = len;
	node->size = ft_itoa(attribs->st_size);
	if (dir && (len = LEN(node->size, 0)) > dir->width.size)
		dir->width.size = len;
}

/*
** iterate through the given directories and add their subdirs to stack,
** add files to file linked list
*/

int		add_dir_name(t_dir *dir, char *current, t_dir *node)
{
	size_t			len;

	node->name = ft_strdup(current);
	if ((len = LEN(current, 0)) > dir->width.name)
		dir->width.name = len;
	node->parent = ft_strdup(dir->parent);
	if ((len = LEN(node->parent, 0)) > dir->width.parent)
		dir->width.parent = len;
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

	len = LEN(dir->parent, 0) + LEN(name, 0) + 1;
	str = ft_strnew(len);
	ft_memcpy((void*)str, dir->parent, LEN(dir->parent, 0));
	ft_strcat(str, "/");
	ft_strcat(str, name);
	return (str);
}

void	harvest_dir(t_ls *ctx, t_dir *dir)
{
	t_list			*dirs;
	t_dir			node;
	struct dirent	*d;
	DIR				*dr;
	struct stat		attribs;

	dirs = NULL;
	if ((dr = opendir((dir->full = form_dir(dir, dir->name)))))
	{
		while ((d = readdir(dr)))
		{
			ft_bzero(&node, sizeof(t_dir));
			if (ERR(lstat((node.full = form_path(dir, d->d_name)), &attribs)))
				continue ;
			harvest_node(ctx, dir, &node, &attribs);
			if (add_dir_name(dir, d->d_name, &node) && GET_RECURSE(ctx->flags))
				ft_lstpushback(&dirs, ft_lstnew((void*)&node, sizeof(t_dir)));
			ft_lstpushback(&dir->files, ft_lstnew((void*)&node, sizeof(t_dir)));
		}
		closedir(dr);
		dirs = ft_lstmergesort(ctx->compare
			, dirs, !GET_REVERSE(ctx->flags), ft_lstsize(dirs));
		dir->files = ft_lstmergesort(ctx->compare
			, dir->files, !GET_REVERSE(ctx->flags), ft_lstsize(dir->files));
		ft_lstmerge(&ctx->stack, dirs);
	}
}

/*
** traverse given directories  and subdirectories, printing at each stage
*/

void	crawl_files(t_ls *ctx)
{
	int		rev;
	t_list	*d;

	rev = !GET_REVERSE(ctx->flags);
	ctx->compare = sort_null;
	GET_SORT_ACCESS(ctx->flags) ? (ctx->compare = sort_access) : NULL;
	GET_SORT_TIME(ctx->flags) ? (ctx->compare = sort_time) : NULL;
	ctx->stack = ft_lstmergesort(ctx->compare, ctx->stack, rev, ft_lstsize(ctx->stack));
	while (ctx->stack)
	{
		d = ft_lsttail(&ctx->stack);
		harvest_dir(ctx, (t_dir*)d->content);
		print_dir(ctx, (t_dir*)d->content);
	}
}
