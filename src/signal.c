/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 19:33:17 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/19 23:08:03 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_sig_handler(int sig)
{
	if (sig != SIGCHLD)
		return ;
	ft_printf("child sighandler: %d\n", g_processes);
	// g_processes -= 1;
	// _exit(1);
}

void	sig_handler(int sig)
{
	// int		i;

	if (sig != SIGINT)
		return ;
	// ft_printf("sig on pid: %d\n", getpid());
	// ft_printf("sighandler: %d\n", g_processes);
	// if (g_processes <= 0)
	// {
	// 	i = 0;
	// 	while (g_environ[i])
	// 		free(g_environ[i++]);
	// 	write(STDOUT, "\nexiting\n", 9);
	// 	exit(1);
	// }
	write(STDOUT, "\n", 1);
	g_processes -= 1;
}
