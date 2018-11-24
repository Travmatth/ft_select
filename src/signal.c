/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 16:37:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/23 17:49:18 by tmatthew         ###   ########.fr       */
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
}

void	sigcont_handler(int sig)
{
	if (sig != SIGCONT)
		return ;
	prepare_tty();
	write_lines();
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
	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigtstp_handler);
	signal(SIGCONT, sigcont_handler);
	signal(SIGWINCH, sigwinch_handler);
}
