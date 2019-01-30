/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/01/29 14:44:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	ft_select_exit(int status)
{
	restore_tty(tty_singleton(NULL));
	exit(status);
}

void	ft_select_err(char *message)
{
	ft_putendl(message);
	exit(1);
}

void	write_args(t_ctx *ctx)
{
	size_t	i;

	i = 0;
	while (i < ctx->argc ? ctx->argc - 1 : 0)
	{
		if (ctx->selected[i])
		{
			write(STDOUT, ctx->argv[i], ctx->lens[i]);
			write(STDOUT, " ", 1);
		}
		i += 1;
	}
}

void	free_args(t_ctx *ctx)
{
	free(ctx->selected);
	free(ctx->lens);
}

int		ft_gputchar(int c)
{
	int		fd;

	fd = fd_singleton(NIL);
	write(fd, &c, 1);
	return (0);
}
