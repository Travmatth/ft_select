/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 16:37:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/19 16:47:57 by tmatthew         ###   ########.fr       */
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
