/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singletons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 16:45:17 by tmatthew          #+#    #+#             */
/*   Updated: 2019/01/30 15:15:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

struct termios	*tty_singleton(struct termios *new)
{
	static struct termios	*ctx;

	if (new != NULL && !ctx)
		ctx = ft_memdup(new, sizeof(struct termios));
	return (ctx);
}

t_ctx			*ctx_singleton(t_ctx *new)
{
	static t_ctx	*ctx;

	if (new != NULL)
		ctx = new;
	return (ctx);
}

int				fd_singleton(int new)
{
	static int	fd;

	if (!NONE(new))
		fd = new;
	return (fd);
}
