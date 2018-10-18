/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:54:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/17 22:11:08 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
** print given files in short format
*/

void	print_multi_files(t_ls *ctx, t_list *files)
{
	unsigned short	files_per_line;
	unsigned short	i;
	t_dir			*n;
	size_t			max;
	t_list			*node;

	get_files_per_line(files, &files_per_line, &max);
	files = sort_line(&files, files_per_line);
	while (ft_lstpeektail(files))
	{
		i = 0;
		while (i++ < files_per_line && (node = ft_lsttail(&files)))
		{
			n = (t_dir*)node->content;
			n->format_str = format_color(ctx, "%-*s ", n);
			ft_printf(n->format_str, (int)max, n->name);
			ft_lstdel(&node, free_dir);
		}
		write(STDOUT, "\n", 1);
	}
	write(STDOUT, "\n", 1);
}

/*
** print given files in short or long format
*/

void	print_files(t_ls *ctx, t_list *files, int *i)
{
	t_dir	*n;
	t_list	*node;

	*i = 1;
	if (!GET_ALL(ctx->flags))
		files = ft_lstfilter(files, find_hidden, free_dir);
	if (GET_LONG(ctx->flags))
	{
		print_long_dir(ctx, files, NULL);
		return ;
	}
	if (!GET_NL(ctx->flags))
		print_multi_files(ctx, files);
	else
		while ((node = ft_lsttail(&files)))
		{
			n = (t_dir*)node->content;
			ft_printf((n->format_str = format_color(ctx, "%s\n", n)), n->name);
			ft_lstdel(&node, free_dir);
		}
}

/*
** print single directory in short format
*/

void	print_single_dir(t_ls *ctx, t_dir *dir)
{
	t_dir	*n;
	t_list	*node;

	node = ft_lsttail(&dir->files);
	while (node)
	{
		n = (t_dir*)node->content;
		n->format_str = format_color(ctx, "%s\n", n);
		ft_printf(n->format_str, n->name);
		ft_lstdel(&node, free_dir);
		node = ft_lsttail(&dir->files);
	}
}

/*
** print given directory in short format
*/

void	print_multi_dir(t_ls *ctx, t_dir *dir)
{
	unsigned short	files_per_line;
	unsigned short	i;
	size_t			max;
	t_dir			*n;
	t_list			*node;

	get_files_per_line(dir->files, &files_per_line, &max);
	dir->files = sort_line(&dir->files, files_per_line);
	while (ft_lstpeektail(dir->files))
	{
		i = 0;
		while (i++ < files_per_line && (node = ft_lsttail(&dir->files)))
		{
			n = (t_dir*)node->content;
			if (n)
			{
				n->format_str = format_color(ctx, "%-*s ", n);
				ft_printf(n->format_str, (int)max, n->name);
			}
			else
				write(STDOUT, " ", 1);
			ft_lstdel(&node, free_dir);
		}
		write(STDOUT, "\n", 1);
	}
}

/*
** print given directory in short or long formats
*/

void	print_dir(t_ls *ctx, t_dir *dir, int *i)
{
	int		widths[4];

	if (!dir->root || (dir->root && ctx->top_lvl_dirs > 1))
	{
		if (*i)
			write(STDOUT, "\n", 1);
		ft_printf("%s:\n", dir->full);
	}
	*i += 1;
	if (dir->denied)
	{
		ft_printf("ls: %s: Permission denied\n", dir->name);
		return ;
	}
	if (GET_LONG(ctx->flags))
	{
		dir->total_out = ft_itoa(dir->total);
		find_widths(dir->files, widths);
		print_long_dir(ctx, dir->files, dir->total_out);
		dir->files = NULL;
		return ;
	}
	GET_NL(ctx->flags) ? print_single_dir(ctx, dir) : print_multi_dir(ctx, dir);
}
