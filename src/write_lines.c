/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 17:35:00 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/27 17:14:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	write_formatted_arg(size_t *i)
{
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
		write_formatted_arg(i);
		ft_putstr_fd(tgetstr("me", NULL), g_fd);
		offset = g_ctx.width - g_ctx.lens[*i];
		ft_putstr_fd(g_ctx.blanks + g_ctx.width - offset + 1, g_fd);
		if (*i % g_ctx.cols == g_ctx.cols - 1)
			write(g_fd, "\n", 1);
		*i += 1;
	}
}

int		write_lines(void)
{
	int		x;
	int		y;
	size_t	i;

	if (!(g_ctx.width = get_term_size())
		|| !g_ctx.width || g_ctx.win_row < g_ctx.rows
		|| (g_ctx.width * g_ctx.cols > g_ctx.win_col))
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_gputchar);
		ft_putstr_fd("error: window too small!", g_fd);
		return (0);
	}
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

void	read_input(void)
{
	char	ctrl_seq[4];
	int		b;

	while (42)
	{
		ft_bzero(ctrl_seq, 4);
		write_lines();
		if (ERR((b = read(g_fd, &ctrl_seq, 4))))
			ft_select_err("invalid command");
		else if (ft_strnequ(CURSOR_UP, ctrl_seq, 4))
			cursor_up();
		else if (ft_strnequ(CURSOR_DOWN, ctrl_seq, 4))
			cursor_down();
		else if (ft_strnequ(CURSOR_LEFT, ctrl_seq, 4))
			cursor_left();
		else if (ft_strnequ(CURSOR_RIGHT, ctrl_seq, 4) || IS_SPACE(ctrl_seq))
			cursor_right(IS_SPACE(ctrl_seq) ? 1 : 0);
		else if (IS_NEWLINE(ctrl_seq))
			break ;
		else if (ft_strnequ(DELETE, ctrl_seq, 4) || (IS_BACKSPACE(ctrl_seq)))
			delete_opt();
		else if (IS_ESC(ctrl_seq))
			ft_select_exit(1);
	}
}
