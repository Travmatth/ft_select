/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/01/29 13:50:27 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	restore_tty(struct termios *tty)
{
	tty->c_lflag &= (ICANON | ECHO | ISIG | IEXTEN | ECHOE | ECHOK | ECHOKE
		| ECHOCTL);
	if (ERR(tcsetattr(fd_singleton(NIL), TCSADRAIN, tty)))
		ft_select_err("tcsetattr");
	tputs(tgetstr("ve", NULL), 1, ft_gputchar);
	tputs(tgetstr("te", NULL), 1, ft_gputchar);
}

int		prepare_tty(void)
{
	char			*tty_id;
	char			*type;
	char			buf[BUFF_SIZE];
	struct termios	t;
	int				fd;

	tty_id = getenv("FTSHELL_TTY");
	fd = fd_singleton(open(tty_id ? tty_id : ttyname(STDIN), O_RDWR));
	if (!OK(fd) || !isatty(fd))
		ft_select_err("Not a terminal device");
	else if (NONE((type = getenv("TERM")
		? getenv("TERM") : getenv("FTSHELL_TERM"))))
		ft_select_err("TERM or substitute not found");
	else if (ERR(tgetent(buf, type)))
		ft_select_err("Term type not valid");
	else if (ERR(tcgetattr(fd, &t)) || NONE(tty_singleton(&t)))
		ft_select_err("tcgetattr");
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (ERR(tcsetattr(fd, TCSANOW, &t)))
		ft_select_err("tcsetattr");
	tputs(tgetstr("ti", NULL), 1, ft_gputchar);
	tputs(tgetstr("vi", NULL), 1, ft_gputchar);
	return (fd);
}

int		main(int argc, char **argv)
{
	t_ctx			*ctx;
	int				fd;
	struct termios	*tty;

	if (argc == 1)
		ft_putstr("error: no params given\n");
	else if ((ctx = ft_memalloc(sizeof(t_ctx))))
	{
		ctx_singleton(ctx);
		fd = prepare_tty();
		tty = tty_singleton(NULL);
		register_signals();
		ctx->argc = (size_t)(argc - 1);
		ctx->argv = &argv[1];
		format_args(ctx);
		write_lines(fd, ctx);
		read_input(fd, ctx);
		restore_tty(tty);
		write_args(ctx);
		free_args(ctx);
	}
	return (0);
}
