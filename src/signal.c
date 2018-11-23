/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 16:37:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/22 17:49:02 by tmatthew         ###   ########.fr       */
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
	signal(SIGTSTP, SIG_DFL);
	restore_tty();
}

void	sigcont_handler(int sig)
{
	if (sig != SIGCONT)
		return ;
	prepare_tty();
	display();
}

void	sigwinch_handler(int sig)
{
	if (sig != SIGWINCH)
		return ;
	ft_dprintf(g_log
		, "width: %zu win_row: %zu rows: %zu cols: %zu win_col: %zu\n"
		, g_ctx.width
		, g_ctx.win_row
		, g_ctx.rows
		, g_ctx.cols
		, g_ctx.win_col);
	
	// clear_display();
	tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_gputchar);
	tputs(tgetstr("cd", NULL), 1, ft_gputchar);
	// display();
}
