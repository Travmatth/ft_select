/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/07 21:24:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	ft_select_err(char *message)
{
	ft_putendl(message);
	exit(1);
}

void	display(int fd, char **argv, t_offset *offsets)
{
	(void)fd;
	(void)argv;
	(void)offsets;
}

int		prepare_tty(t_tty *term)
{
	char			*tty;
	int				fd;
	char			*type;
	char			buf[BUFF_SIZE];
	struct termios	t;

	if ((tty = getenv("FTSHELL_TTY")))
		fd = open(tty, O_WRONLY);
	else
		fd = STDOUT;
	if (!OK(fd) || !isatty(fd))
		ft_select_err("Not a terminal device");
	else if (NONE((type = fd == 1 ? getenv("TERM") : getenv("FTSHELL_TERM"))))
		ft_select_err("Not a terminal device");
	else if (ERR(tgetent(buf, type)))
		ft_select_err("Term type not valid");
	else if (ERR(tcgetattr(fd, &term->attr)))
		ft_select_err("tcgetattr");
	t = term->attr;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (ERR(tcsetattr(fd, TCSADRAIN, &term->attr)))
		ft_select_err("tcsetattr");
	return (fd);
}

int		main(int argc, char **argv)
{
	t_tty		tty;
	t_offset	offsets;
	int			fd;

	if (!(fd = prepare_tty(&tty)))
		return (1);
	ft_bzero(&offsets, sizeof(offsets));
	format_args(argc, argv, &offsets);
	display(fd, argv, &offsets);
	if (ERR(tcsetattr(fd, TCSADRAIN, &tty.attr)))
		ft_select_err("tcsetattr");
	return (0);
}
