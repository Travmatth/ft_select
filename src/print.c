/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:54:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/02 20:21:01 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
**	print dir contents
*/

static	char 	*permissions = "-------";

void	format_long_listing(t_ls *ctx, t_dir *file)
{
	(void)ctx;
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
	// need to owner and group names, time last modified
	// ft_dprintf(STDOUT, "%s %d %s %s %d %s %s", permissions, file->attribs.st_nlink, )
}

void	print_dir(t_ls *ctx, t_dir *dir)
{
	t_dir	*file;

	while (dir->files)
	{
		file = (t_dir*)ft_lsttail(&dir->files)->content;
		if (!GET_LONG(ctx->flags))
		{
			write(STDOUT, file->name, LEN(file->name, 0));
			if (file->dir)
				write(STDOUT, "\t", 1);
			else
				write(STDOUT, " ", 1);
			continue;
		}
		// format_long_listing(ctx->dir);
	}
}

void	print_files(t_ls *ctx)
{
	(void)ctx;
}
