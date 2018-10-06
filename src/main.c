/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/06 15:42:27 by tmatthew         ###   ########.fr       */
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
	{'G', 9}
};

void	ft_ls_usage(void)
{
}

void	locate_file(t_ls *ctx, char *dirname)
{
	DIR			*dir;
	t_dir		d;

	if ((dir = opendir(dirname)))
	{
		ft_bzero((void*)&d, sizeof(t_dir));
		d.name = dirname;
		d.parent = dirname;
		d.root = 1;
		ctx->top_lvl_dirs += 1;
		closedir(dir);
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
	while (i < 10)
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
		ft_ls_usage();
}

int		parse_opts(t_ls *ctx, int argc, char **argv)
{
	int		n;
	size_t	i;
	size_t	len;

	n = 1;
	while (n < argc)
	{
		i = 1;
		len = LEN(argv[n], 0);
		if (argv[n][0] != '-')
			break ;
		if (!len || len < 2)
			ft_ls_usage();
		while (i < len)
			parse_opt(ctx, argv[n][i++]);
		n += 1;
	}
	while (n < argc)
		locate_file(ctx, argv[n++]);
	return (1);
}

int		main(int argc, char **argv)
{
	t_ls	ctx;

	ft_bzero(&ctx, sizeof(t_ls));
	if (argc == 1)
		locate_file(&ctx, ".");
	else
		parse_opts(&ctx, argc, argv);
	crawl_files(&ctx);
	return (0);
}
