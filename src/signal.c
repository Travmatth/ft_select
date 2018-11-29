/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 16:37:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/28 17:29:12 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	sigint_handler(int sig)
{
	struct termios	*tty;
	t_ctx			*ctx;

	if (sig != SIGINT
		|| NONE((ctx = ctx_singleton(NULL)))
		|| NONE((tty = tty_singleton(NULL))))
		return ;
	restore_tty(tty);
	free_args(ctx);
	exit(1);
}

void	sigtstp_handler(int sig)
{
	struct termios	*tty;

	if (sig != SIGTSTP || NONE((tty = tty_singleton(NULL))))
		return ;
	restore_tty(tty);
	signal(SIGTSTP, SIG_DFL);
	ioctl(fd_singleton(NIL), TIOCSTI, "\x1A");
}

void	sigcont_handler(int sig)
{
	t_ctx	*ctx;
	int		fd;

	if (sig != SIGCONT
		|| NONE((fd = fd_singleton(NIL)))
		|| NONE((ctx = ctx_singleton(NULL))))
		return ;
	prepare_tty();
	register_signals();
	write_lines(fd, ctx);
}

void	sigwinch_handler(int sig)
{
	t_ctx	*ctx;
	int		fd;

	if (sig != SIGWINCH
		|| NONE((fd = fd_singleton(NIL)))
		|| NONE((ctx = ctx_singleton(NULL))))
		return ;
	tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_gputchar);
	tputs(tgetstr("cd", NULL), 1, ft_gputchar);
	write_lines(fd, ctx);
}

void	register_signals(void)
{
	signal(SIGABRT, sigint_handler);
	signal(SIGTERM, sigint_handler);
	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigtstp_handler);
	signal(SIGCONT, sigcont_handler);
	signal(SIGWINCH, sigwinch_handler);
}
