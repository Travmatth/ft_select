/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 13:35:39 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/13 18:06:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

int		ft_gputchar(int c)
{
	write(g_fd, &c, 1);
	return (0);
}

void	write_arg(int argc, char **argv, int *i, t_ctx *ctx)
{
	int		x;
	int		y;
	size_t	offset;

	while (*i < argc && *i % ctx->cols < ctx->cols)
	{
		x = *i % ctx->cols;
		y = *i / ctx->cols;
		if (ctx->selected[*i])
		{
			ft_putstr_fd(tgetstr("so", NULL), g_fd);
			ft_putstr_fd(tgetstr("us", NULL), g_fd);
		}
		ft_putstr_fd(argv[*i], g_fd);
		ft_putstr_fd(tgetstr("me", NULL), g_fd);
		offset = ctx->width - ctx->lens[*i];
		ft_putstr_fd(ctx->blanks + ctx->width - offset + 1, g_fd);
		if (*i % ctx->cols == ctx->cols - 1)
			write(g_fd, "\n", 1);
		*i += 1;
	}
}

int		small_display(int argc, char **argv, t_ctx *ctx)
{
	(void)argc;
	(void)argv;
	(void)ctx;
	return (0);
}

int		write_lines(int argc, char **argv, t_ctx *ctx, char ctrl_seq[4])
{
	int		i;
	int		x;
	int		y;

	if (!(ctx->width = get_term_size(argc, argv, ctx)))
		return (small_display(argc, argv, ctx));
	ft_bzero(ctrl_seq, 4);
	i = ((int)ctx->width + 2) * sizeof(char);
	if (!(ctx->blanks = (char*)ft_memalloc(i)))
		return ;
	ft_memset(ctx->blanks, ' ', ctx->width + 1);
	i = 0;
	ft_putstr_fd(tgetstr("ho", NULL), g_fd);
	ft_putstr_fd(tgetstr("cd", NULL), g_fd);
	while (i < argc)
		write_arg(argc, argv, &i, ctx);
	x = ctx->focus / ctx->cols;
	y = (ctx->focus % ctx->cols);
	tputs(tgoto(tgetstr("cm", NULL), y * ctx->width, x), 1, ft_gputchar);
	return (1);
}

void	display(int argc, char **argv, t_ctx *ctx)
{
	char	ctrl_seq[4];
	int		b;

	while (42)
	{
		if (!write_lines(argc, argv, ctx, ctrl_seq))
			break ;
		else if (ERR((b = read(g_fd, &ctrl_seq, 4))))
			ft_select_err("invalid command");
		else if (ft_strnequ(CURSOR_UP, ctrl_seq, 4))
			cursor_up(argc, argv, ctx);
		else if (ft_strnequ(CURSOR_DOWN, ctrl_seq, 4))
			cursor_down(argc, argv, ctx);
		else if (ft_strnequ(CURSOR_LEFT, ctrl_seq, 4))
			cursor_left(argc, argv, ctx);
		else if (ft_strnequ(CURSOR_RIGHT, ctrl_seq, 4))
			cursor_right(argc, argv, ctx, 0);
		else if (ctrl_seq[0] == ' ' && !ctrl_seq[1])
			cursor_right(argc, argv, ctx, 1);
		else if (ctrl_seq[0] == '\n')
			break ;
	}
}
