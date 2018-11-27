/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/26 18:14:16 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	ft_select_exit(int status)
{
	restore_tty();
	exit(status);
}

void	ft_select_err(char *message)
{
	ft_putendl(message);
	exit(1);
}

void	write_args(void)
{
	size_t	i;

	i = 0;
	while (i < g_ctx.argc ? g_ctx.argc - 1 : 0)
	{
		if (g_ctx.selected[i])
		{
			write(STDOUT, g_ctx.argv[i], g_ctx.lens[i]);
			write(STDOUT, " ", 1);
		}
		i += 1;
	}
	free(g_ctx.selected);
	free(g_ctx.blanks);
	free(g_ctx.lens);
}

int		ft_gputchar(int c)
{
	write(g_fd, &c, 1);
	return (0);
}
