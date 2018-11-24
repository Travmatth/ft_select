/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 15:41:18 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/23 17:11:21 by tmatthew         ###   ########.fr       */
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

int					g_fd;
struct termios		g_tty;
t_ctx				g_ctx;

/*
** events.c
*/

void				cursor_up(void);
void				cursor_down(void);
void				cursor_left(void);
void				cursor_right(int selected);
void				delete_opt(void);

/*
** write_lines.c
*/

int					write_lines(void);
void				read_input(void);

/*
** parse.c
*/

void				format_args(void);
size_t				get_term_size(void);

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

void				prepare_tty(void);
void				restore_tty(void);

/*
** utils.c
*/

void				ft_select_err(char *message);
void				ft_select_exit(int argc);
void				free_args(void);
int					ft_gputchar(int c);
#endif
