/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 16:42:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/11 17:15:55 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
** print a given node in long format
*/

void	print_long(t_ls *ctx, t_dir *n, t_list *lst)
{
	char	*perms;
	int		widths[4];

	find_widths(lst, widths);
	perms = format_permissions(n);
	n->date_str = format_date(n->date);
	n->name_str = format_name(n);
	n->format_str = format_color(ctx, "%s  %*s %*s  %*s  %*s %s ", n);
	ft_printf(n->format_str, perms, widths[0], n->links
		, widths[1], n->owner_name
		, widths[2], n->owner_group
		, widths[3], n->size
		, n->date_str, n->name, n->name_str);
	free(perms);
}

/*
** print given directory with long formatting
*/

void	print_long_dir(t_ls *ctx, t_list *lst, char *totals)
{
	t_list	*node;
	t_dir	*n;

	node = ft_lsttail(&lst);
	if (totals && node)
		ft_printf("total %s\n", totals);
	while (node)
	{
		n = (t_dir*)node->content;
		while (node)
		{
			n = (t_dir*)node->content;
			if (n->denied)
				ft_printf("ls: %s: Permission denied", n->name);
			else
				print_long(ctx, n, lst);
			write(STDOUT, "\n", 1);
			ft_lstdel(&node, free_dir);
			node = ft_lsttail(&lst);
		}
		if (node)
			write(STDOUT, "\n", 1);
	}
}

/*
** add color code to short format strings
*/

char	*template_swap_first(char *template, char *fmt)
{
	char	*out;
	char	*spec;
	size_t	start;
	size_t	end;

	start = ft_strchr(template, '%') - template;
	end = ft_strchr(template, 's') - template + 1;
	spec = ft_strsub(template, (unsigned int)start, end);
	fmt = ft_strjoin(fmt, spec);
	out = ft_strjoin(fmt, "{eoc}");
	free(fmt);
	fmt = out;
	out = ft_swap(template, spec, fmt);
	free(fmt);
	free(spec);
	return (out);
}

/*
** add color to long format strings
*/

char	*template_make_last(char *template, char *fmt, t_dir *node)
{
	char	*out;

	out = ft_strjoin(fmt, "%s");
	if (S_ISLNK(node->mode))
		fmt = ft_strjoin(out, "{eoc} -> %s");
	else
		fmt = ft_strjoin(out, "{eoc}");
	free(out);
	out = ft_strjoin(template, fmt);
	free(fmt);
	return (out);
}

/*
** find maximum width of given files stack name members
*/

void	get_files_per_line(t_list *files
							, unsigned short *files_per_line
							, void **max)
{
	struct winsize	w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	*max = ft_lstfoldl(get_max_width, files);
	*files_per_line = (w.ws_col ? w.ws_col : 80)
		/ (*max ? **(size_t**)max : 10);
	free(max);
}
