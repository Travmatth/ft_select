/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crawl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:56:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/30 18:59:58 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ft_ls.h"

/*
** iterate through the given directories and add their subdirs to stack,
** add files to file linked list
*/

void	harvest_dir(t_ls *ctx, t_dir *dir)
{
	t_list	*dirs;
	t_dir	node;
	struct dirent	*d;
	DIR		*dr;

	dirs = NULL;
	if ((dr = opendir(dir->name)))
	{
		while ((d = readdir(dr)))
		{
			node.name = d->d_name;
			lstat(d->d_name, &node.attribs);
			if ((node.dir = S_ISDIR(node.attribs.st_mode)))
				ft_lstpushback(&dirs
					, ft_lstnew((void*)&node, sizeof(t_dir)));
			else
				ft_lstpushback(&dir->files, ft_lstnew((void*)&node
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
**	free dir
*/

void	free_dir(t_list *dir)
{
	(void)dir;
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
		print_dir(ctx, (t_dir*)d->content);
		free_dir(d);
	}
}
