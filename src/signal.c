/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 19:33:17 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/20 15:21:26 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_sig_handler(int sig)
{
	if (sig != SIGCHLD)
		return ;
	g_processes -= 1;
	_exit(1);
}

void	sig_handler(int sig)
{
	int		i;

	if (sig != SIGINT)
		return ;
	if (g_processes <= 0)
	{
		i = 0;
		while (g_environ[i])
			free(g_environ[i++]);
		write(STDOUT, "\nexiting\n", 9);
		exit(1);
	}
	write(STDOUT, "\n", 1);
}
