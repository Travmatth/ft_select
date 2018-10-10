/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:08:47 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/09 16:05:09 by tmatthew         ###   ########.fr       */
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
# include <sys/ioctl.h>
# include <sys/types.h>
# include <errno.h>

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
# define SET_NL(v) (BITSET(v, 10))

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
# define GET_NL(v) (BITTEST(v, 10))

# define USR_OR_STICK (S_ISUID | S_IXUSR)
# define GRP_OR_STICK (S_ISGID | S_IXGRP)
# define OTH_OR_STICK (S_IXOTH | S_ISVTX)

typedef struct	s_lsopt
{
	char		name;
	int			position;
}				t_lsopt;


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
	char		*total_out;
	char		*date_str;
	char		*name_str;
	size_t		name_width;
	size_t		parent_width;
	t_list		*files;
	long		atime;
	long		atime_nsec;
	long		mtime;
	long		mtime_nsec;
	int			dir;
	int			root;
	int			denied;
	mode_t		mode;
	int			total;
}				t_dir;

typedef struct	s_ls
{
	char		flags[BITNSLOTS(11)];
	t_list		*stack;
	int			(*compare)(void *first, void *second);
	int			top_lvl_dirs;
}				t_ls;

void			*get_max_width(void *final, t_list *elem, size_t i, int *stop);
int				find_files(t_list *elem);
int				find_hidden(t_list *elem);
void			free_dir(t_dir *dir);
void			remove_hidden(t_list *elem);
void			*find_link(void *final, t_list *elem, size_t i, int *stop);
void			*find_name(void *final, t_list *elem, size_t i, int *stop);
void			*find_grp(void *final, t_list *elem, size_t i, int *stop);
void			*find_size(void *final, t_list *elem, size_t i, int *stop);
int				*find_widths(t_list *lst, int widths[5]);
void			crawl_files(t_ls *ctx);
void			print_dir(t_ls *ctx, t_dir *dir);
void			print_files(t_ls *ctx, t_list *files);
void			harvest_node(t_ls *ctx
							, t_dir *node
							, struct stat *attribs);
int				sort_access(void *first, void *second);
int				sort_time(void *first, void *second);
int				sort_alpha(void *first, void *second);
int				sort_null(void *first, void *second);
#endif
