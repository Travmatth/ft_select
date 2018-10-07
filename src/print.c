/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:54:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/06 18:58:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
**	print dir contents
*/

static	char 	*permissions = "----------";

int		find_hidden(t_list *elem)
{
	t_dir	*d;

	d = (t_dir*)elem->content;
	return (ft_strequ(".", d->name) || ft_strequ("..", d->name));
}

void	free_dir(t_dir *dir)
{
	(void)dir;
}

void	remove_hidden(t_list *elem)
{
	free_dir((t_dir*)elem->content);
}

char	*format_permissions(t_dir *file)
{
	char *out;

	out = ft_strdup(permissions);
	out[0] = S_ISDIR(file->mode) ? 'd' : '-';
	out[1] = file->mode & S_IRUSR ? 'r' : '-';
	out[2] = file->mode & S_IWUSR ? 'w' : '-';
	out[3] = file->mode & S_IXUSR ? 'x' : '-';
	out[4] = file->mode & S_IRGRP ? 'r' : '-';
	out[5] = file->mode & S_IWGRP ? 'w' : '-';
	out[6] = file->mode & S_IXGRP ? 'x' : '-';
	out[7] = file->mode & S_IROTH ? 'r' : '-';
	out[8] = file->mode & S_IWOTH ? 'w' : '-';
	out[9] = file->mode & S_IXOTH ? 'x' : '-';
	return (out);
}

char	*format_date(char *date)
{
	char *out;
	char **parts;

	parts = ft_strsplit(date, ' ');
	out = ft_strnew(12);
	parts[4][4] = '\0';
	ft_memcpy((void*)out, parts[1], 3);
	out[3] = ' ';
	if (parts[2][1])
	{
		out[4] = parts[2][0];
		out[5] = parts[2][1];
	}
	out[6] = ' ';
	ft_memcpy((void*)(out + 7), parts[3], 5);
	return (out);
}

void	print_long_listing(t_dir *dir)
{
	t_list	*node;
	t_dir	*n;
	char	*perms;
	int		widths[4];

	find_widths(dir->files, widths);
	node = ft_lsttail(&dir->files);
	ft_printf("total: %s\n", dir->total);
	while (node)
	{
		n = (t_dir*)node->content;
		while (node)
		{
			n = (t_dir*)node->content;
			perms = format_permissions(n);
			ft_printf("%s  %*s %*s  %*s  %*s %s %s", perms, widths[0], n->links
				, widths[1], n->owner_name
				, widths[2], n->owner_group
				, widths[3], n->size
				, format_date(n->date), n->name);
			write(STDOUT, "\n", 1);
			node = ft_lsttail(&dir->files);
		}
		write(STDOUT, "\n", 1);
	}
}

void	print_dir(t_ls *ctx, t_dir *dir)
{
	t_list			*node;
	t_dir			*n;
	unsigned short	i;
	struct winsize	w;
	unsigned short	files_per_line;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if (!GET_ALL(ctx->flags))
		dir->files = ft_lstfilter(dir->files, find_hidden, remove_hidden);
	if (GET_LONG(ctx->flags))
	{
		print_long_listing(dir);
		return ;
	}
	files_per_line = (w.ws_col ? w.ws_col : 80)
		/ (dir->name_width ? dir->name_width : LEN(dir->name, 0));
	node = ft_lsttail(&dir->files);
	if (!dir->root || (dir->root && ctx->top_lvl_dirs > 1))
	{
		write(STDOUT, dir->full, LEN(dir->full, 0));
		write(STDOUT, ":\n", 1);
	}
	while (node)
	{
		i = 0;
		n = (t_dir*)node->content;
		while (node && i++ < files_per_line)
		{
			n = (t_dir*)node->content;
			ft_printf("%-*s ", (int)dir->name_width, n->name);
			node = ft_lsttail(&dir->files);
		}
		write(STDOUT, "\n\n", 2);
	}
}
