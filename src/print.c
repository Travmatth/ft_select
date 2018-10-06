/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:54:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/05 19:16:40 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
**	print dir contents
*/

static	char 	*permissions = "-------";

char	*format_long_listing(t_dir *file)
{
	permissions[0] = S_ISDIR(file->mode) ? 'd' : '-';
	permissions[1] = file->mode & S_IRUSR ? 'r' : '-';
	permissions[2] = file->mode & S_IWUSR ? 'w' : '-';
	permissions[3] = file->mode & S_IXUSR ? 'x' : '-';
	permissions[4] = file->mode & S_IRGRP ? 'r' : '-';
	permissions[5] = file->mode & S_IWGRP ? 'w' : '-';
	permissions[6] = file->mode & S_IXGRP ? 'x' : '-';
	permissions[7] = file->mode & S_IROTH ? 'r' : '-';
	permissions[8] = file->mode & S_IWOTH ? 'w' : '-';
	permissions[9] = file->mode & S_IXOTH ? 'x' : '-';
	return (permissions);
}

void	print_long_listing(t_ls *ctx, t_dir *dir)
{
	(void)ctx;
	(void)dir;
}

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

void	print_dir(t_ls *ctx, t_dir *dir)
{
	t_list			*node;
	t_dir			*n;
	unsigned short	i;
    struct winsize	w;
	unsigned short	files_per_line;

	if (GET_LONG(ctx->flags))
		print_long_listing(ctx, dir);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	files_per_line = (w.ws_col ? w.ws_col : 80) / dir->width.name;
	// write(STDOUT, dir->name, LEN(dir->name, 0));
	// write(STDOUT, "\n", 1);
	dir->files = GET_ALL(ctx->flags) 
		? dir->files
		: ft_lstfilter(dir->files, find_hidden, remove_hidden);
	node = ft_lsttail(&dir->files);
	while (node)
	{
		i = 0;
		while (node && i++ < files_per_line)
		{
			n = (t_dir*)node->content;
			ft_printf("%-*s ", (int)dir->width.name, n->name);
			node = ft_lsttail(&dir->files);
		}
		write(STDOUT, "\n", 1);
	}
	// need to owner and group names, time last modified
	// ft_dprintf(STDOUT, "%s %d %s %s %d %s %s", permissions, file->attribs.st_nlink, )
}