/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/26 23:17:26 by tmatthew         ###   ########.fr       */
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

int		parse_opts(t_ls *ctx, int argc, char **argv)
{
	size_t	i;

	while (argc)
	{
		i = 0;
		while (i < 10)
		{
			if (ft_strequ(argv[i], g_lsopts[i].name))
			{
				BITSET(ctx->flags, g_lsopts[i].position);
				if (g_lsopts[i].name[1] == 'f')
					BITSET(ctx->flags, 2);
			}
			else if (locate_file(ctx, argv[i]))
				continue ;
			else if (i == 10)
				return (0);
			i += 1;
		}
		argc -= 1;
	}
	return (1);
}

int		main(int argc, char **argv)
{
	t_ls	ctx;

	if (argc == 1)
		ft_ls_usage(NULL);
	else if (parse_opts(&ctx, argc, argv))
		crawl_files(&ctx);
	else
		ft_ls_usage(argv);
	return (0);
}
