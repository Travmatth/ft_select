/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 17:35:00 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/28 17:18:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	write_formatted_arg(int fd, t_ctx *ctx, size_t *i)
{
	if (ctx->selected[*i])
	{
		ft_putstr_fd(tgetstr("so", NULL), fd);
		ft_putstr_fd(tgetstr("us", NULL), fd);
		ft_putstr_fd(ctx->argv[*i], fd);
	}
	else if (ctx->focus == *i)
	{
		ft_putstr_fd(tgetstr("us", NULL), fd);
		write(fd, ctx->argv[*i], 1);
		ft_putstr_fd(tgetstr("me", NULL), fd);
		write(fd, ctx->argv[*i] + 1, ctx->lens[*i] - 1);
	}
	else
		ft_putstr_fd(ctx->argv[*i], fd);
}

void	write_arg(int fd, t_ctx *ctx, size_t *i)
{
	size_t	x;
	size_t	y;
	size_t	offset;

	while (*i < ctx->argc && *i % ctx->cols < ctx->cols)
	{
		x = *i % ctx->cols;
		y = *i / ctx->cols;
		write_formatted_arg(fd, ctx, i);
		ft_putstr_fd(tgetstr("me", NULL), fd);
		offset = ctx->width - ctx->lens[*i];
		ft_putstr_fd(ctx->blanks + ctx->width - offset + 1, fd);
		if (*i % ctx->cols == ctx->cols - 1)
			write(fd, "\n", 1);
		*i += 1;
	}
}

int		write_lines(int fd, t_ctx *ctx)
{
	int		x;
	int		y;
	size_t	i;

	if (!(ctx->width = get_term_size(fd, ctx))
		|| !ctx->width || ctx->win_row < ctx->rows
		|| (ctx->width * ctx->cols > ctx->win_col))
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_gputchar);
		ft_putstr_fd("error: window too small!", fd);
		return (0);
	}
	i = ((int)ctx->width + 2) * sizeof(char);
	if (!(ctx->blanks = (char*)ft_memalloc(i)))
		return (0);
	ft_memset(ctx->blanks, ' ', ctx->width + 1);
	i = 0;
	ft_putstr_fd(tgetstr("ho", NULL), fd);
	ft_putstr_fd(tgetstr("cd", NULL), fd);
	while (i < ctx->argc)
		write_arg(fd, ctx, &i);
	x = ctx->focus / ctx->cols;
	y = (ctx->focus % ctx->cols);
	tputs(tgoto(tgetstr("cm", NULL), y * ctx->width, x), 1, ft_gputchar);
	return (1);
}

void	read_input(int fd, t_ctx *ctx)
{
	char	ctrl_seq[4];
	int		b;

	while (42)
	{
		ft_bzero(ctrl_seq, 4);
		write_lines(fd, ctx);
		if (ERR((b = read(fd, &ctrl_seq, 4))))
			ft_select_err("invalid command");
		else if (ft_strnequ(CURSOR_UP, ctrl_seq, 4))
			cursor_up(ctx);
		else if (ft_strnequ(CURSOR_DOWN, ctrl_seq, 4))
			cursor_down(ctx);
		else if (ft_strnequ(CURSOR_LEFT, ctrl_seq, 4))
			cursor_left(ctx);
		else if (ft_strnequ(CURSOR_RIGHT, ctrl_seq, 4) || IS_SPACE(ctrl_seq))
			cursor_right(ctx, IS_SPACE(ctrl_seq) ? TRUE : FALSE);
		else if (IS_NEWLINE(ctrl_seq))
			break ;
		else if (ft_strnequ(DELETE, ctrl_seq, 4) || (IS_BACKSPACE(ctrl_seq)))
			delete_opt(ctx);
		else if (IS_ESC(ctrl_seq))
			ft_select_exit(1);
	}
}
