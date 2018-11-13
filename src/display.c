/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 13:35:39 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/12 20:00:19 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

int		ft_gputchar(int c)
{
	write(g_fd, &c, 1);
	return (0);
}

void	write_arg(int argc, char **argv, int *i, t_offset *offsets)
{
	int		x;
	int		y;
	size_t	offset;

	while (*i < argc && *i % offsets->cols < offsets->cols)
	{
		x = *i % offsets->cols;
		y = *i / offsets->cols;
		if (offsets->selected[*i])
		{
			ft_putstr_fd(tgetstr("so", NULL), g_fd);
			ft_putstr_fd(tgetstr("us", NULL), g_fd);
		}
		ft_putstr_fd(argv[*i], g_fd);
		ft_putstr_fd(tgetstr("me", NULL), g_fd);
		offset = offsets->width - offsets->lens[*i];
		ft_putstr_fd(offsets->blanks + offsets->width - offset + 1, g_fd);
		if (*i % offsets->cols == offsets->cols - 1)
			write(g_fd, "\n", 1);
		*i += 1;
	}
}

void	display_cursor(t_offset *offsets)
{
	int		x;
	int		y;

	x = offsets->focus / offsets->cols;
	y = (offsets->focus % offsets->cols);
	tputs(tgoto(tgetstr("cm", NULL), y * offsets->width, x), 1, ft_gputchar);
}

void	write_lines(int argc, char **argv, t_offset *offsets)
{
	int		i;

	i = ((int)offsets->width + 2) * sizeof(char);
	if (!(offsets->blanks = (char*)ft_memalloc(i)))
		return ;
	ft_memset(offsets->blanks, ' ', offsets->width + 1);
	i = 0;
	ft_putstr_fd(tgetstr("ho", NULL), g_fd);
	ft_putstr_fd(tgetstr("cd", NULL), g_fd);
	while (i < argc)
		write_arg(argc, argv, &i, offsets);
	display_cursor(offsets);
}

void	display(int argc, char **argv, t_offset *offsets)
{
	char	ctrl_seq[5];
	int		b;

	while (42)
	{
		write_lines(argc, argv, offsets);
		ft_bzero(ctrl_seq, 5);
		if (ERR((b = read(g_fd, &ctrl_seq, 4))))
			ft_select_err("invalid command");
		else if (OK(b))
		{
			if (ft_strequ(CURSOR_UP, ctrl_seq))
				cursor_up(argc, argv, offsets);
			else if (ft_strequ(CURSOR_DOWN, ctrl_seq))
				cursor_down(argc, argv, offsets);
			else if (ft_strequ(CURSOR_LEFT, ctrl_seq))
				cursor_left(argc, argv, offsets);
			else if (ft_strequ(CURSOR_RIGHT, ctrl_seq))
				cursor_right(argc, argv, offsets, 0);
			else if (ctrl_seq[0] == ' ' && !ctrl_seq[1])
				cursor_right(argc, argv, offsets, 1);
			else if (ctrl_seq[0] == '\n')
				break ;
		}
	}
}
