/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 16:37:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/27 17:13:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	sigint_handler(int sig)
{
	if (sig != SIGINT)
		return ;
	restore_tty();
	exit(1);
}

void	sigtstp_handler(int sig)
{
	if (sig != SIGTSTP)
		return ;
	restore_tty();
	signal(SIGTSTP, SIG_DFL);
	ioctl(g_fd, TIOCSTI, "\x1A");
}

void	sigcont_handler(int sig)
{
	if (sig != SIGCONT)
		return ;
	prepare_tty();
	write_lines();
	register_signals();
}

void	sigwinch_handler(int sig)
{
	if (sig != SIGWINCH)
		return ;
	tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_gputchar);
	tputs(tgetstr("cd", NULL), 1, ft_gputchar);
	write_lines();
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
