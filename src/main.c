/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/11 17:56:06 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

int gd;

void	ft_select_err(char *message)
{
	ft_putendl(message);
	exit(1);
}

int	ft_gputchar(int c)
{
	write(gd, &c, 1);
	return 0;
}

void	write_arg(int fd, char **argv, int i, t_offset *offsets)
{
	int		x;
	int		y;
	size_t	offset;

	x = i % offsets->cols;
	y = i / offsets->cols;
	if (offsets->selected[i])
	{
		ft_putstr_fd(tgetstr("so", NULL), fd);
		ft_putstr_fd(tgetstr("us", NULL), fd);
	}
	ft_putstr_fd(argv[i], fd);
	ft_putstr_fd(tgetstr("me", NULL), fd);
	offset = offsets->width - offsets->lens[i];
	// offset = offset ? offset : offsets->width;
	ft_putstr_fd(offsets->blanks + offsets->width - offset, fd);
	if (i % offsets->cols == offsets->cols - 1)
		write(fd, "\n", 1);
}

void	display(int fd, int argc, char **argv, t_offset *offsets)
{
	int		i;

	i = ((int)offsets->width + 2) * sizeof(char);
	if (!(offsets->blanks = (char*)ft_memalloc(i)))
		return ;
	ft_memset(offsets->blanks, ' ', offsets->width + 1);
	i = 0;
	ft_putstr_fd(tgetstr("ho", NULL), fd);
	ft_putstr_fd(tgetstr("cd", NULL), fd);
	while (i < argc)
		while (i < argc && i % offsets->cols < offsets->cols)
			write_arg(fd, argv, i++, offsets);
	tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_gputchar);
}

int		prepare_tty(t_tty *term)
{
	char			*tty_id;
	int				fd;
	char			*type;
	char			buf[BUFF_SIZE];
	struct termios	t;

	tty_id = getenv("FTSHELL_TTY");
	fd = tty_id ? open(tty_id, O_WRONLY) : STDOUT;
	gd = fd;
	if (!OK(fd) || !isatty(fd))
		ft_select_err("Not a terminal device");
	else if (NONE((type = getenv(fd == 1 ? "TERM" : "FTSHELL_TERM"))))
		ft_select_err("TERM or substitute not found");
	else if (ERR(tgetent(buf, type)))
		ft_select_err("Term type not valid");
	else if (ERR(tcgetattr(fd, &term->attr)))
		ft_select_err("tcgetattr");
	t = term->attr;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (ERR(tcsetattr(fd, TCSADRAIN, &t)))
		ft_select_err("tcsetattr");
	tgetstr("vi", NULL);
	tgetstr("ti", NULL);
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
	format_args(fd, argc - 1, &argv[1], &offsets);
	display(fd, argc - 1, &argv[1], &offsets);
	if (ERR(tcsetattr(fd, TCSADRAIN, &tty.attr)))
		ft_select_err("tcsetattr");
	return (0);
}
