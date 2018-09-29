/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/29 12:01:41 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

/*
** -l; List in long format.  If the output is to a terminal, a total sum for
** all the file sizes is output on a line before the long listing.
** -R: Recursively list subdirectories encountered.
** -a: Include directory entries whose names begin with a dot (.).
** -r: Reverse the order of the sort
** -t: Sort by time modified
** -u: Sort by time of last access
** -f: Output is not sorted.  This option turns on the -a option.
** -g: display the group name in the long (-l) format output
** (the owner name is suppressed)
** -d: Directories are listed as plain files
** -G: Enable colorized output
*/

t_lsopt	g_lsopts[] =
{
	{"-l", 0},
	{"-R", 1},
	{"-a", 2},
	{"-r", 3},
	{"-t", 4},
	{"-u", 5},
	{"-f", 6},
	{"-g", 7},
	{"-d", 8},
	{"-G", 9}
};

int		sort_access(void *first, void *second)
{
	(void)first;
	(void)second;
	return (1);
}

int		sort_time(void *first, void *second)
{
	(void)first;
	(void)second;
	return (1);
}

int		sort_null(void *first, void *second)
{
	(void)first;
	(void)second;
	return (1);
}

/*
**	files add to file list
**	dirs
**		push reverse sorted dir onto stack
**		also add to parents file stack
*/

void	harvest_dir(t_dir *dir)
{
}

/*
**	print dir contents
*/

void	print_dir(t_dir *dir)
{
}

/*
**	free dir
*/

void	free_dir(t_list *dir)
{
}

/*
** reverse sort dirs
** while stack remains
**	pop dir
**	harvest dir
**		files add to file list
**		dirs
**			push reverse sorted dir onto stack
**			also add to parents file stack
**	print dir contents
**	free dir
*/

void	crawl_files(t_ls *ctx)
{
	int		rev;
	t_list	*dirs;
	t_list	*d;
	int		(*f)(void *first, void *second);

	rev = !GET_REVERSE(ctx->flags);
	f = GET_SORT_ACCESS(ctx->flags) ? sort_access : sort_null;
	f = GET_SORT_TIME(ctx->flags) ? sort_time : f;
	dirs = ft_lstmergesort(f, ctx->stack, rev, ft_lstsize(ctx->stack));
	while (dirs)
	{
		d = ft_lsthead(&dirs);
		harvest_dir((t_dir*)d->content);
		print_dir((t_dir*)d->content);
		free_dir(d);
	}
}

void	ft_ls_usage(void)
{
}

int		locate_file(t_ls *ctx, char *dirname, int *i)
{
	DIR		*dir;
	t_dir	d;

	*i += 1;
	if ((dir = opendir(dirname)))
	{
		lstat(dirname, &d.d);
		d.dir = 1;
		d.name = dirname;
		closedir(dir);
		ft_lstpushback(&ctx->stack, ft_lstnew(&d, sizeof(t_dir)));
		return (1);
	}
	return (0);
}

int		parse_opts(t_ls *ctx, int argc, char **argv)
{
	int		i;
	int		n;

	n = 1;
	while (n < argc)
	{
		i = 0;
		while (i < 10)
		{
			if (ft_strequ(argv[n], g_lsopts[i].name))
			{
				BITSET(ctx->flags, g_lsopts[i].position);
				if (g_lsopts[i++].name[1] == 'f')
					BITSET(ctx->flags, 2);
				break ;
			}
			else if (i == 9 && SET_NO_SORT(ctx->flags))
				SET_ALL(ctx->flags);
			else if (i == 9 && locate_file(ctx, argv[n], &i))
				continue ;
			else if (i == 9)
				return (0);
			i += 1;
		}
		n += 1;
	}
	return (1);
}

int		main(int argc, char **argv)
{
	t_ls	ctx;

	if (argc == 1)
		ft_ls_usage();
	else if (parse_opts(&ctx, argc, argv))
		crawl_files(&ctx);
	else
		ft_ls_usage();
	return (0);
}
