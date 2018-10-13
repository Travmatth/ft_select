/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:54:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/12 18:09:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
** print given files in short format
*/

void	print_multiline_files(t_ls *ctx, t_list *node, t_list *files)
{
	unsigned short	files_per_line;
	unsigned short	i;
	t_dir			*n;
	void			*max;

	i = 0;
	get_files_per_line(files, &files_per_line, &max);
	while (node && i++ < files_per_line)
	{
		n = (t_dir*)node->content;
		n->format_str = format_color(ctx, "%-*s ", n);
		ft_printf(n->format_str, (int)max, n->name);
		ft_lstdel(&node, free_dir);
		node = ft_lsttail(&files);
	}
	write(STDOUT, "\n\n", 2);
}

/*
** print given files in short or long format
*/

void	print_files(t_ls *ctx, t_list *files)
{
	t_dir	*n;
	t_list	*node;

	if (!GET_ALL(ctx->flags))
		files = ft_lstfilter(files, find_hidden, free_dir);
	if (GET_LONG(ctx->flags))
	{
		print_long_dir(ctx, files, NULL);
		return ;
	}
	node = ft_lsttail(&files);
	while (node)
	{
		n = (t_dir*)node->content;
		if (GET_NL(ctx->flags))
		{
			ft_printf((n->format_str = format_color(ctx, "%s\n", n)), n->name);
			ft_lstdel(&node, free_dir);
			node = ft_lsttail(&files);
		}
		else
			print_multiline_files(ctx, node, files);
	}
}

/*
** print single directory in short format
*/

void	print_single_dir(t_ls *ctx, t_dir *dir, t_list **node)
{
	t_dir			*n;

	n = (t_dir*)(*node)->content;
	n->format_str = format_color(ctx, "%s\n", n);
	ft_printf(n->format_str, n->name);
	ft_lstdel(node, free_dir);
	*node = ft_lsttail(&dir->files);
}

/*
** print given directory in short format
*/

void	print_multi_dir(t_ls *ctx, t_dir *dir, t_list **node)
{
	unsigned short	files_per_line;
	unsigned short	i;
	void			*max;
	t_dir			*n;

	i = 0;
	get_files_per_line(dir->files, &files_per_line, &max);
	while (*node && i++ < files_per_line)
	{
		n = (t_dir*)(*node)->content;
		n->format_str = format_color(ctx, "%-*s ", n);
		ft_printf(n->format_str, (int)dir->name_width, n->name);
		ft_lstdel(node, free_dir);
		*node = ft_lsttail(&dir->files);
	}
	write(STDOUT, "\n", 1);
	if (*node)
		write(STDOUT, "\n", 1);
}

/*
** print given directory in short or long formats
*/

void	print_dir(t_ls *ctx, t_dir *dir)
{
	t_list	*node;
	int		widths[4];

	if (!dir->root || (dir->root && ctx->top_lvl_dirs > 1))
		ft_printf("\n%s:\n", dir->full);
	if (dir->denied)
	{
		ft_printf("ls: %s: Permission denied\n", dir->name);
		return ;
	}
	if (!GET_ALL(ctx->flags))
		dir->files = ft_lstfilter(dir->files, find_hidden, free_dir);
	if (GET_LONG(ctx->flags))
	{
		dir->total_out = ft_itoa(dir->total);
		find_widths(dir->files, widths);
		print_long_dir(ctx, dir->files, dir->total_out);
		dir->files = NULL;
		return ;
	}
	node = ft_lsttail(&dir->files);
	while (node)
		GET_NL(ctx->flags)
			? print_single_dir(ctx, dir, &node)
			: print_multi_dir(ctx, dir, &node);
}
