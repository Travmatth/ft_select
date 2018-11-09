/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/08 18:58:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	ft_select_err(char *message)
{
	ft_putendl(message);
	exit(1);
}

void	display(int fd, int argc, char **argv, t_offset *offsets)
{
	int		i;
	size_t	current;
	char	*blanks;
	char	*line;

	i = (int)offsets->width;
	if (!(blanks = (char*)ft_memalloc(i * sizeof(char)))
		|| !(line = (char*)ft_memalloc(i * offsets->cols * sizeof(char))))
		return ;
	ft_memset(blanks, ' ', offsets->width);
	i = 1;
	current = 0;
	ft_putstr_fd(tgetstr("ho", NULL), fd);
	ft_putstr_fd(tgetstr("cd", NULL), fd);
	while (i < argc)
	{
		while (i < argc && i % offsets->cols < offsets->cols)
		{
			current = (i - 1) * offsets->width + (i == 1 ? 0 : 1);
			ft_memcpy(line + current, argv[i], offsets->lens[i]);
			current += offsets->lens[i];
			ft_memcpy(line + current, blanks, offsets->width - offsets->lens[i] + 1);
			current += offsets->width - offsets->lens[i] + 1;
			i += 1;
		}
		ft_putstr_fd(line, fd);
	}
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
	display(fd, argc, argv, &offsets);
	if (ERR(tcsetattr(fd, TCSADRAIN, &tty.attr)))
		ft_select_err("tcsetattr");
	return (0);
}
