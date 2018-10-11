/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crawl_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 16:39:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/11 16:41:08 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

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
