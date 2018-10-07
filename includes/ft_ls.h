/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:08:47 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/06 18:55:53 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <fcntl.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include "../libftprintf/srcs/includes/ft_printf.h"
# include <dirent.h>
# include <sys/stat.h>
#include <sys/ioctl.h>

# define SET_LONG(v) (BITSET(v, 0))
# define SET_RECURSE(v) (BITSET(v, 1))
# define SET_ALL(v) (BITSET(v, 2))
# define SET_REVERSE(v) (BITSET(v, 3))
# define SET_SORT_TIME(v) (BITSET(v, 4))
# define SET_SORT_ACCESS(v) (BITSET(v, 5))
# define SET_NO_SORT(v) (BITSET(v, 6))
# define SET_DISPL_GRP(v) (BITSET(v, 7))
# define SET_NO_RECURSE(v) (BITSET(v, 8))
# define SET_COLOR_OUT(v) (BITSET(v, 9))
# define SET_DIR(v) (BITSET(v, 10))

# define GET_LONG(v) (BITTEST(v, 0))
# define GET_RECURSE(v) (BITTEST(v, 1))
# define GET_ALL(v) (BITTEST(v, 2))
# define GET_REVERSE(v) (BITTEST(v, 3))
# define GET_SORT_TIME(v) (BITTEST(v, 4))
# define GET_SORT_ACCESS(v) (BITTEST(v, 5))
# define GET_NO_SORT(v) (BITTEST(v, 6))
# define GET_DISPL_GRP(v) (BITTEST(v, 7))
# define GET_NO_RECURSE(v) (BITTEST(v, 8))
# define GET_COLOR_OUT(v) (BITTEST(v, 9))
# define GET_DIR(v) (BITTEST(v, 10))

typedef struct	s_lsopt
{
	char		name;
	int			position;
}				t_lsopt;

// typedef struct	s_width
// {
// 	size_t		name;
// 	size_t		parent;
// 	size_t		link;
// 	size_t		size;
// 	size_t		owner_name;
// 	size_t		owner_group;
// 	size_t		file_size;
// 	size_t		day;
// }				t_width;

#include <sys/types.h>

typedef struct	s_dir
{
	char		*name;
	char		*parent;
	char		*full;
	char		*links;
	char		*owner_name;
	char		*owner_group;
	char		*size;
	char		*date;
	size_t		name_width;
	size_t		parent_width;
	t_list		*files;
	long		atime;
	long		mtime;
	int			dir;
	int			root;
	mode_t		mode;
	char		*total;
}				t_dir;

typedef struct	s_ls
{
	char		flags[BITNSLOTS(11)];
	t_list		*stack;
	int			(*compare)(void *first, void *second);
	int			top_lvl_dirs;
}				t_ls;

void			*find_link(void *final, t_list *elem, size_t i, int *stop);
void			*find_name(void *final, t_list *elem, size_t i, int *stop);
void			*find_grp(void *final, t_list *elem, size_t i, int *stop);
void			*find_size(void *final, t_list *elem, size_t i, int *stop);
int				*find_widths(t_list *lst, int widths[5]);
void			ft_ls_usage(void);
void			crawl_files(t_ls *ctx);
void			print_dir(t_ls *ctx, t_dir *dir);
void			harvest_node(t_ls *ctx
							, t_dir *node
							, struct stat *attribs);
int				sort_access(void *first, void *second);
int				sort_time(void *first, void *second);
int				sort_alpha(void *first, void *second);
#endif
