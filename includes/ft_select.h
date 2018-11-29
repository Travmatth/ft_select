/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 15:41:18 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/28 17:27:19 by tmatthew         ###   ########.fr       */
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
# include <signal.h>

/*
** https://stackoverflow.com/questions/4130048/recognizing-arrow-keys-with-stdin
** https://en.wikipedia.org/wiki/ANSI_escape_code
*/

# define CURSOR_UP "\x1b[A"
# define CURSOR_DOWN "\x1b[B"
# define CURSOR_LEFT "\x1b[D"
# define CURSOR_RIGHT "\x1b[C"
# define DELETE "\x1b[3~"
# define ESC 0x1b
# define BACKSPACE 0x7f

# define IS_BACKSPACE(x) (x[0] == BACKSPACE && !x[1])
# define IS_SPACE(x) (x[0] == ' ' && !x[1])
# define IS_NEWLINE(x) (x[0] == '\n' && !x[1])
# define IS_ESC(x) (x[0] == ESC && !x[1])

typedef struct		s_ctx
{
	size_t			win_col;
	size_t			win_row;
	char			*blanks;
	char			**argv;
	size_t			*lens;
	short			*selected;
	size_t			width;
	size_t			rows;
	size_t			cols;
	size_t			focus;
	size_t			argc;
}					t_ctx;

/*
** events.c
*/

void				cursor_up(t_ctx *ctx);
void				cursor_down(t_ctx *ctx);
void				cursor_left(t_ctx *ctx);
void				cursor_right(t_ctx *ctx, int selected);
void				delete_opt(t_ctx *ctx);

/*
** write_lines.c
*/

int					write_lines(int fd, t_ctx *ctx);
void				read_input(int fd, t_ctx *ctx);

/*
** parse.c
*/

void				format_args(t_ctx *ctx);
size_t				get_term_size(int fd, t_ctx *ctx);

/*
** signal.c
*/

void				sigint_handler(int sig);
void				sigtstp_handler(int sig);
void				sigcont_handler(int sig);
void				sigwinch_handler(int sig);
void				register_signals(void);

/*
** main.c
*/

int					prepare_tty(void);
void				restore_tty(struct termios *tty);

/*
** utils.c
*/

void				ft_select_err(char *message);
void				ft_select_exit(int argc);
void				write_args(t_ctx *ctx);
int					ft_gputchar(int c);
void				free_args(t_ctx *ctx);

/*
** singletons.c
*/

struct termios		*tty_singleton(struct termios *new);
t_ctx				*ctx_singleton(t_ctx *new);
int					fd_singleton(int new);
#endif
