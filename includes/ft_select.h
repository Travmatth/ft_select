/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 15:41:18 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/14 14:46:42 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include "../libftprintf/srcs/includes/ft_printf.h"
# include <termcap.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <errno.h>
# include <termios.h>

/*
** https://stackoverflow.com/questions/4130048/recognizing-arrow-keys-with-stdin
** https://en.wikipedia.org/wiki/ANSI_escape_code
*/

# define CURSOR_UP "\x1b[A"
# define CURSOR_DOWN "\x1b[B"
# define CURSOR_LEFT "\x1b[D"
# define CURSOR_RIGHT "\x1b[C"

int					g_fd;
typedef struct		s_ctx
{
	int				win_col;
	int				win_row;
	char			*blanks;
	size_t			*lens;
	short			*selected;
	size_t			width;
	int				rows;
	int				cols;
	int				focus;
}					t_ctx;

typedef struct		s_tty
{
	struct termios	attr;
}					t_tty;

/*
** events.c
*/

void				cursor_up(int argc, char **argv, t_ctx *ctx);
void				cursor_down(int argc, char **argv, t_ctx *ctx);
void				cursor_left(int argc, char **argv, t_ctx *ctx);
void				cursor_right(int argc
								, char **argv
								, t_ctx *ctx
								, int selected);

/*
** display.c
*/

void				display(int argc, char **argv, t_ctx *ctx);
int					ft_gputchar(int c);

/*
** parse.c
*/

void				format_args(int argc, char **argv, t_ctx *ctx);
size_t				get_term_size(int argc, char **argv, t_ctx *ctx);

/*
** main.c
*/

void				ft_select_err(char *message);
#endif
