/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 13:35:39 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/20 18:49:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

int		ft_gputchar(int c)
{
	write(g_fd, &c, 1);
	return (0);
}

void	write_arg(size_t *i)
{
	size_t	x;
	size_t	y;
	size_t	offset;

	while (*i < g_ctx.argc && *i % g_ctx.cols < g_ctx.cols)
	{
		x = *i % g_ctx.cols;
		y = *i / g_ctx.cols;
		if (g_ctx.selected[*i])
		{
			ft_putstr_fd(tgetstr("so", NULL), g_fd);
			ft_putstr_fd(tgetstr("us", NULL), g_fd);
			ft_putstr_fd(g_ctx.argv[*i], g_fd);
		}
		else if (g_ctx.focus == (size_t)*i)
		{
			ft_putstr_fd(tgetstr("us", NULL), g_fd);
			write(g_fd, g_ctx.argv[*i], 1);
			ft_putstr_fd(tgetstr("me", NULL), g_fd);
			write(g_fd, g_ctx.argv[*i] + 1, g_ctx.lens[*i] - 1);
		}
		else
			ft_putstr_fd(g_ctx.argv[*i], g_fd);
		ft_putstr_fd(tgetstr("me", NULL), g_fd);
		offset = g_ctx.width - g_ctx.lens[*i];
		ft_putstr_fd(g_ctx.blanks + g_ctx.width - offset + 1, g_fd);
		if (*i % g_ctx.cols == g_ctx.cols - 1)
			write(g_fd, "\n", 1);
		*i += 1;
	}
}

int		small_display(void)
{
	tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_gputchar);
	ft_putstr_fd("error: window too small!", g_fd);
	return (0);
}

int		write_lines(void)
{
	int		x;
	int		y;
	size_t	i;

	g_ctx.width = get_term_size();
	if (!g_ctx.width || g_ctx.win_row < g_ctx.rows
		|| (g_ctx.width * g_ctx.cols > g_ctx.win_col))
		return (small_display());
	i = ((int)g_ctx.width + 2) * sizeof(char);
	if (!(g_ctx.blanks = (char*)ft_memalloc(i)))
		return (0);
	ft_memset(g_ctx.blanks, ' ', g_ctx.width + 1);
	i = 0;
	ft_putstr_fd(tgetstr("ho", NULL), g_fd);
	ft_putstr_fd(tgetstr("cd", NULL), g_fd);
	while (i < g_ctx.argc)
		write_arg(&i);
	x = g_ctx.focus / g_ctx.cols;
	y = (g_ctx.focus % g_ctx.cols);
	tputs(tgoto(tgetstr("cm", NULL), y * g_ctx.width, x), 1, ft_gputchar);
	return (1);
}

void	display(void)
{
	char	ctrl_seq[4];
	int		b;

	while (42)
	{
		errno = 0;
		ft_bzero(ctrl_seq, 4);
		ft_putstr_fd("here\n", g_log);
		if (!write_lines())
			break ;
		else if (ERR((b = read(g_fd, &ctrl_seq, 4))))
			ft_select_err("invalid command");
		else if (NONE(b) && errno == EINTR)
			exit(0);
		else if (ft_strnequ(CURSOR_UP, ctrl_seq, 4))
			cursor_up();
		else if (ft_strnequ(CURSOR_DOWN, ctrl_seq, 4))
			cursor_down();
		else if (ft_strnequ(CURSOR_LEFT, ctrl_seq, 4))
			cursor_left();
		else if (ft_strnequ(CURSOR_RIGHT, ctrl_seq, 4))
			cursor_right(0);
		else if (ctrl_seq[0] == ' ' && !ctrl_seq[1])
			cursor_right(1);
		else if (ctrl_seq[0] == '\n' && !ctrl_seq[1])
			break ;
		else if (ft_strnequ(DELETE, ctrl_seq, 4)
			|| (ctrl_seq[0] == BACKSPACE && !ctrl_seq[1]))
			delete_opt();
		else if (ctrl_seq[0] == ESC && !ctrl_seq[1])
			ft_select_exit(1);
		else if (errno == EINTR)
			ft_dprintf(g_log, "signal");
	}
}
