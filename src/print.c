/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:54:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/09 15:35:19 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
**	print dir contents
*/

static	char 	*permissions = "----------";

char	*format_permissions(t_dir *file)
{
	char *out;

	out = ft_strdup(permissions);
	out[0] = S_ISDIR(file->mode) ? 'd' : '-';
	out[0] = S_ISLNK(file->mode) ? 'l' : out[0];
	out[0] = S_ISCHR(file->mode) ? 'c' : out[0];
	out[1] = file->mode & S_IRUSR ? 'r' : '-';
	out[2] = file->mode & S_IWUSR ? 'w' : '-';
	out[3] = file->mode & S_IXUSR ? 'x' : '-';
	out[3] = (file->mode & (USR_OR_STICK)) == S_ISUID ? 'S' : out[3];
	out[3] = (file->mode & (USR_OR_STICK)) == (USR_OR_STICK) ? 's' : out[3];
	out[4] = file->mode & S_IRGRP ? 'r' : '-';
	out[5] = file->mode & S_IWGRP ? 'w' : '-';
	out[6] = file->mode & S_IXGRP ? 'x' : '-';
	out[6] = (file->mode & (GRP_OR_STICK)) == S_ISGID ? 'S' : out[6];
	out[6] = (file->mode & (GRP_OR_STICK)) == (GRP_OR_STICK) ? 's' : out[6];
	out[7] = file->mode & S_IROTH ? 'r' : '-';
	out[8] = file->mode & S_IWOTH ? 'w' : '-';
	out[9] = file->mode & S_IXOTH ? 'x' : '-';
	out[9] = (file->mode & (OTH_OR_STICK)) == S_ISVTX ? 'T' : out[9];
	out[9] = (file->mode & (OTH_OR_STICK)) == (OTH_OR_STICK) ? 't' : out[9];
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
	else
	{
		out[4] = ' ';
		out[5] = parts[2][0];
	}
	out[6] = ' ';
	ft_memcpy((void*)(out + 7), parts[3], 5);
	return (out);
}

char	*format_name(t_dir *node)
{
	char	buf[256];
	char	*out;
	ssize_t	bytes;
	char	*name;

	if (!S_ISLNK(node->mode))
		return (ft_strdup(node->name));
	ft_bzero(buf, 256);
	name = node->full ? node->full : node->name;
	bytes = readlink(name, buf, 255);
	ft_asprintf(&out, "%s -> %s", name, buf);
	return (out);
}

void	print_long_listings(t_list *lst, char *totals)
{
	t_list	*node;
	t_dir	*n;
	char	*perms;
	int		widths[4];

	find_widths(lst, widths);
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
			{
				perms = format_permissions(n);
				n->date_str = format_date(n->date);
				n->name_str = format_name(n);
				ft_printf("%s  %*s %*s  %*s  %*s %s %s", perms, widths[0], n->links
					, widths[1], n->owner_name
					, widths[2], n->owner_group
					, widths[3], n->size
					, n->date_str, n->name_str);
			}
			write(STDOUT, "\n", 1);
			node = ft_lsttail(&lst);
		}
		if (node)
			write(STDOUT, "\n", 1);
	}
}

void	print_files(t_ls *ctx, t_list *files)
{
	t_dir			*n;
	unsigned short	i;
	struct winsize	w;
	unsigned short	files_per_line;
	void			*max;
	t_list			*node;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if (!GET_ALL(ctx->flags))
		files = ft_lstfilter(files, find_hidden, remove_hidden);
	max = ft_lstfoldl(get_max_width, files);
	files_per_line = (w.ws_col ? w.ws_col : 80) / (max ? *(size_t*)max : 10);
	if (GET_LONG(ctx->flags))
	{
		print_long_listings(files, NULL);
		return ;
	}
	node = ft_lsttail(&files);
	while (node)
	{
		i = 0;
		n = (t_dir*)node->content;
		if (GET_NL(ctx->flags))
		{
			ft_printf("%s\n", n->name);
			node = ft_lsttail(&files);
		}
		else
		{
			while (node && i++ < files_per_line)
			{
				n = (t_dir*)node->content;
				ft_printf("%-*s ", (int)max, n->name);
				node = ft_lsttail(&files);
			}
			write(STDOUT, "\n\n", 2);
		}
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
	if (!dir->root || (dir->root && ctx->top_lvl_dirs > 1))
		ft_printf("\n%s:\n", dir->full);
	if (dir->denied)
	{
		ft_printf("ls: %s: Permission denied\n", dir->name);
		return ;
	}
	if (!GET_ALL(ctx->flags))
		dir->files = ft_lstfilter(dir->files, find_hidden, remove_hidden);
	if (GET_LONG(ctx->flags))
	{
		print_long_listings(dir->files, (dir->total_out = ft_itoa(dir->total)));
		return ;
	}
	files_per_line = (w.ws_col ? w.ws_col : 80)
		/ (dir->name_width ? dir->name_width : LEN(dir->name, 0));
	node = ft_lsttail(&dir->files);
	while (node)
	{
		i = 0;
		n = (t_dir*)node->content;
		if (GET_NL(ctx->flags))
		{
			ft_printf("%s\n", n->name);
			node = ft_lsttail(&dir->files);
		}
		else
		{
			while (node && i++ < files_per_line)
			{
				n = (t_dir*)node->content;
				ft_printf("%-*s ", (int)dir->name_width, n->name);
				node = ft_lsttail(&dir->files);
			}
			write(STDOUT, "\n", 1);
			if (node)
				write(STDOUT, "\n", 1);
		}
	}
}
