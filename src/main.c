/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/08 23:40:15 by tmatthew         ###   ########.fr       */
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
	{'l', 0},
	{'R', 1},
	{'a', 2},
	{'r', 3},
	{'t', 4},
	{'u', 5},
	{'f', 6},
	{'g', 7},
	{'d', 8},
	{'G', 9},
	{'1', 10}
};

void	ft_ls_usage(char opt)
{
	write(STDOUT, "s: illegal option -- ", 22);
	write(STDOUT, &opt, 1);
	write(STDOUT, "\nusage: ls [-GRadfglrtu1] [file ...]\n", 38);
	exit(1);
}

void	locate_file(t_ls *ctx, char *dirname, char *files)
{
	DIR			*dir;
	t_dir		d;
	struct stat		attribs;

	*files = 1;
	ctx->top_lvl_dirs += 1;
	ft_bzero((void*)&d, sizeof(t_dir));
	d.name = dirname;
	d.parent = dirname;
	d.root = 1;
	d.dir = 1;
	if ((dir = opendir(dirname)))
	{
		if (!ERR(lstat(dirname, &attribs)))
		{
			d.mtime = attribs.st_mtimespec.tv_sec;
			d.mtime_nsec = attribs.st_mtimespec.tv_nsec;
			d.atime = attribs.st_atimespec.tv_sec;
			d.atime_nsec = attribs.st_atimespec.tv_nsec;
			closedir(dir);
			ft_lstpushback(&ctx->stack, ft_lstnew(&d, sizeof(t_dir)));
		}
	}
	else if (errno == EACCES)
	{
		d.denied = 1;
		ft_lstpushback(&ctx->stack, ft_lstnew(&d, sizeof(t_dir)));
	}
	else if (!ERR(lstat(dirname, &attribs)))
	{
		d.total += attribs.st_blocks;
		harvest_node(ctx, &d, &attribs);
		ft_lstpushback(&ctx->stack, ft_lstnew(&d, sizeof(t_dir)));
	}
	else
		ft_printf("ls: %s: No such file or directory\n", dirname);
}

void	parse_opt(t_ls *ctx, char opt)
{
	int		i;
	int		found;

	found = 0;
	i = 0;
	while (i < 11)
	{
		if (opt == g_lsopts[i].name)
		{
			BITSET(ctx->flags, g_lsopts[i].position);
			if (g_lsopts[i++].name == 'f')
				BITSET(ctx->flags, 2);
			found = 1;
			break ;
		}
		i += 1;
	}
	if (!found)
		ft_ls_usage(opt);
}

void	parse_opts(t_ls *ctx, int argc, char **argv)
{
	int		n;
	size_t	i;
	size_t	len;
	char	stop;

	n = 1;
	stop = 0;
	while (n < argc)
	{
		i = 1;
		len = LEN(argv[n], 0);
		ft_printf("opt: %s\n", argv[n]);
		if (ft_strequ("--", argv[n]))
		{
			n += 1;
			break ;
		}
		else if (ft_strequ("-", argv[n]) || argv[n][0] != '-')
			break ;
		while (i < len)
			parse_opt(ctx, argv[n][i++]);
		n += 1;
	}
	stop = 0;
	if (n < argc)
		while (n < argc)
			locate_file(ctx, argv[n++], &stop);
	else if (!stop)
		locate_file(ctx, ".", &stop);
}

int		main(int argc, char **argv)
{
	t_ls	ctx;

	ft_bzero(&ctx, sizeof(t_ls));
	parse_opts(&ctx, argc, argv);
	crawl_files(&ctx);
	return (0);
}
