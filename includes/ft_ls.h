/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:08:47 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/17 19:57:22 by tmatthew         ###   ########.fr       */
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
	char		*format_str;
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

/*
** widths.c
*/

int				*find_widths(t_list *lst, int widths[4]);

/*
** sort.c
*/

int				sort_access(void *first, void *second);
int				sort_time(void *first, void *second);
int				sort_alpha(void *first, void *second);
int				sort_null(void *first, void *second);
t_list			*sort_line(t_list **nodes, unsigned short rows);

/*
** crawl_utils.c
*/

int				add_dir_name(t_dir *dir, char *current, t_dir *node);
char			*form_dir(t_dir *dir, char *name);
char			*form_path(t_dir *dir, char *name);
void			add_to_total(t_ls *ctx, char *name, t_dir *dir, int size);
void			deduplicate_node(t_list *node);

/*
** crawl.c
*/

void			set_root_dir(t_ls *ctx, t_dir *dir, char *dirname, char *files);
void			harvest_node(t_ls *ctx, t_dir *node, struct stat *attribs);
void			harvest_dir_nodes(t_ls *ctx
								, t_dir *dir
								, t_list **dirs
								, DIR *dr);
void			harvest_dir(t_ls *ctx, t_dir *dir);
void			crawl_files(t_ls *ctx);

/*
** format.c
*/

char			*format_color(t_ls *ctx, char *template, t_dir *node);
char			*format_permissions(t_dir *file);
char			*format_date(char *date);
char			*format_name(t_dir *node);

/*
** print.c
*/

void			print_multi_files(t_ls *ctx, t_list *files);
void			print_files(t_ls *ctx, t_list *files, int *i);
void			print_single_dir(t_ls *ctx, t_dir *dir);
void			print_multi_dir(t_ls *ctx, t_dir *dir);
void			print_dir(t_ls *ctx, t_dir *dir, int *i);

/*
** print_utils.c
*/

void			print_long(t_ls *ctx, t_dir *n, int widths[4]);
void			print_long_dir(t_ls *ctx, t_list *lst, char *totals);
char			*template_swap_first(char *template, char *fmt);
char			*template_make_last(char *template, char *fmt, t_dir *node);
void			get_files_per_line(t_list *files
									, unsigned short *files_per_line
									, size_t *max);

/*
** utils.c
*/

void			set_dir_status(t_ls *ctx, t_dir *node, struct stat *attribs);
int				find_hidden(t_list *elem);
void			free_dir(void *d, size_t len);
int				find_files(t_list *elem);
void			*get_max_width(void *final, t_list *elem, size_t i, int *stop);
#endif
