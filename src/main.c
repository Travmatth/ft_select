/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/29 23:15:50 by tmatthew         ###   ########.fr       */
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

/*
** stat.ST_ATIME | Time of last access.
*/

int		sort_access(void *first, void *second)
{
	t_dir *f;
	t_dir *s;

	f = (t_dir*)first;
	s = (t_dir*)second;
	return (f->attribs.st_atime - s->attribs.st_atime);
}

/*
** stat.ST_MTIME | Time of last modification.
*/

/*
** stat.ST_CTIME | On Unix systems is the time of the last metadata change
*/

int		sort_time(void *first, void *second)
{
	t_dir *f;
	t_dir *s;

	f = (t_dir*)first;
	s = (t_dir*)second;
	return (f->attribs.st_mtime - s->attribs.st_mtime);
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
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("."); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
  
    closedir(dr);   
*/

void	harvest_dir(t_ls *ctx, t_dir *dir)
{
	(void)ctx;
	// hold discovered directories
	// t_list	*dirs;
	struct dirent	*d;
	DIR		*dr;
	// struct stat	attribs;

	if ((dr = opendir(dir->name)))
	{
		while ((d = readdir(dr)))
		{
			continue ;
		}
		closedir(dr);
	}
	//
	// dirs = ft_lstmergesort(f, ctx->stack, rev, ft_lstsize(ctx->stack));
	// while (dirs)
	// {
	// 	d = ft_lsttail(&dirs);
	// 	harvest_dir((t_dir*)d->content);
	// 	print_dir((t_dir*)d->content);
	// 	free_dir(d);
	// }
	// DIR		*dir;
	// t_dir	d;

	// if ((dir = opendir(dirname)))
	// {
	// 	lstat(dirname, &d.attribs);
	// 	d.dir = 1;
	// 	d.name = dirname;
	// 	closedir(dir);
	// 	ft_lstpushback(&ctx->stack, ft_lstnew(&d, sizeof(t_dir)));
	// 	return (1);
	// }
	// return (0);
}

/*
**	print dir contents
*/

void	print_dir(t_dir *dir)
{
	(void)dir;
}

/*
**	free dir
*/

void	free_dir(t_list *dir)
{
	(void)dir;
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

	rev = !GET_REVERSE(ctx->flags);
	ctx->compare = sort_null;
	GET_SORT_ACCESS(ctx->flags) ? (ctx->compare = sort_access) : NULL;
	GET_SORT_TIME(ctx->flags) ? (ctx->compare = sort_time) : NULL;
	dirs = ft_lstmergesort(ctx->compare, ctx->stack, rev, ft_lstsize(ctx->stack));
	while (dirs)
	{
		d = ft_lsttail(&dirs);
		harvest_dir(ctx, (t_dir*)d->content);
		print_dir((t_dir*)d->content);
		free_dir(d);
	}
}

void	ft_ls_usage(void)
{
}

int		locate_file(t_ls *ctx, char *dirname)
{
	DIR		*dir;
	t_dir	d;

	if ((dir = opendir(dirname)))
	{
		lstat(dirname, &d.attribs);
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
			if (i == 9 && SET_NO_SORT(ctx->flags))
				SET_ALL(ctx->flags);
			if (i == 9 && !locate_file(ctx, argv[n]))
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
