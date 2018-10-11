/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:54:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/10 23:14:11 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static	char	*g_permissions = "----------";

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

char	*format_color(t_ls *ctx, char *template, t_dir *node)
{
	char	*fmt;
	char	*out;

	if (!GET_COLOR_OUT(ctx->flags))
		return (template);
	else if (S_ISLNK(node->mode))
		fmt = "{purple}";
	else if (S_ISDIR(node->mode))
		fmt = "{b_blue}";
	else if (S_ISBLK(node->mode))
		fmt = "{blue}";
	else if (S_ISCHR(node->mode))
		fmt = "{yellow}";
	else if (S_ISSOCK(node->mode))
		fmt = "{green}";
	else if (S_ISFIFO(node->mode))
		fmt = "{b_cyan}";
	else if (node->mode & S_IXUSR)
		fmt = "{red}";
	else if (S_ISREG(node->mode))
		return (ft_strdup(template));
	if (ft_count_char(template, '%') <= 2)
		out = template_swap_first(template, fmt);
	else
		out = template_make_last(template, fmt, node);
	return (out);
}

char	*format_permissions(t_dir *file)
{
	char *out;

	out = ft_strdup(g_permissions);
	out[0] = S_ISDIR(file->mode) ? 'd' : '-';
	out[0] = S_ISLNK(file->mode) ? 'l' : out[0];
	out[0] = S_ISBLK(file->mode) ? 'b' : out[0];
	out[0] = S_ISCHR(file->mode) ? 'c' : out[0];
	out[0] = S_ISSOCK(file->mode) ? 's' : out[0];
	out[0] = S_ISFIFO(file->mode) ? 'p' : out[0];
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
	char	*out;
	char	**parts;
	size_t	i;

	parts = ft_strsplit(date, ' ');
	out = ft_strnew(12);
	parts[4][4] = '\0';
	ft_memcpy((void*)out, parts[1], 3);
	out[3] = ' ';
	out[4] = parts[2][1] ? parts[2][0] : ' ';
	out[5] = parts[2][1] ? parts[2][1] : parts[2][0];
	out[6] = ' ';
	ft_memcpy((void*)(out + 7), parts[3], 5);
	i = 4;
	while (i)
		free(parts[i--]);
	free(parts[0]);
	free(parts);
	return (out);
}

char	*format_name(t_dir *node)
{
	char	buf[256];
	ssize_t	bytes;
	char	*name;

	if (!S_ISLNK(node->mode))
		return (ft_strdup(node->name));
	ft_bzero(buf, 256);
	name = node->full ? node->full : node->name;
	bytes = readlink(name, buf, 255);
	return (ft_strdup(buf));
}

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

void	print_long_listings(t_ls *ctx, t_list *lst, char *totals)
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

void	get_files_per_line(t_list *files, unsigned short *files_per_line, void **max)
{
	struct winsize	w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	*max = ft_lstfoldl(get_max_width, files);
	*files_per_line = (w.ws_col ? w.ws_col : 80) / (*max ? **(size_t**)max : 10);
	free(max);
}

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

void	print_files(t_ls *ctx, t_list *files)
{
	t_dir			*n;
	t_list			*node;

	if (!GET_ALL(ctx->flags))
		files = ft_lstfilter(files, find_hidden, free_dir);
	if (GET_LONG(ctx->flags))
	{
		print_long_listings(ctx, files, NULL);
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

void	print_dir(t_ls *ctx, t_dir *dir)
{
	t_list			*node;
	t_dir			*n;
	unsigned short	i;
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
		dir->files = ft_lstfilter(dir->files, find_hidden, free_dir);
	if (GET_LONG(ctx->flags))
	{
		print_long_listings(dir->files, (dir->total_out = ft_itoa(dir->total)));
		return ;
	}
	get_files_per_line(files, &files_per_line);
	node = ft_lsttail(&dir->files);
	while (node)
	{
		i = 0;
		n = (t_dir*)node->content;
		if (GET_NL(ctx->flags))
		{
			n->format_str = format_color(ctx, "%s\n", n);
			ft_printf(n->format_str, n->name);
			ft_lstdel(&node, free_dir);
			node = ft_lsttail(&dir->files);
		}
		else
		{
			while (node && i++ < files_per_line)
			{
				n = (t_dir*)node->content;
				n->format_str = format_color(ctx, "%-*s ", n);
				ft_printf(n->format_str, (int)dir->name_width, n->name);
				ft_lstdel(&node, free_dir);
				node = ft_lsttail(&dir->files);
			}
			write(STDOUT, "\n", 1);
			if (node)
				write(STDOUT, "\n", 1);
		}
	}
}
