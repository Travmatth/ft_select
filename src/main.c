/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/23 17:54:12 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	restore_tty(void)
{
	if (ERR(tcsetattr(g_fd == STDIN ? STDOUT : g_fd, TIOCSTI, &g_tty)))
		ft_select_err("tcsetattr");
	tputs(tgetstr("ve", NULL), 1, ft_gputchar);
	tputs(tgetstr("te", NULL), 1, ft_gputchar);
}

void	prepare_tty(void)
{
	char			*tty_id;
	char			*type;
	char			buf[BUFF_SIZE];
	struct termios	t;

	tty_id = getenv("FTSHELL_TTY");
	g_fd = tty_id ? open(tty_id, O_RDWR) : STDOUT;
	if (!OK(g_fd) || !isatty(g_fd))
		ft_select_err("Not a terminal device");
	else if (NONE((type = getenv("TERM")
		? getenv("TERM") : getenv("FTSHELL_TERM"))))
		ft_select_err("TERM or substitute not found");
	else if (ERR(tgetent(buf, type)))
		ft_select_err("Term type not valid");
	else if (ERR(tcgetattr(g_fd == STDOUT ? STDIN : g_fd, &g_tty)))
		ft_select_err("tcgetattr");
	ft_memcpy(&t, &g_tty, sizeof(struct termios));
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (ERR(tcsetattr(g_fd == STDIN ? STDOUT : g_fd, TCSANOW, &t)))
		ft_select_err("tcsetattr");
	tputs(tgetstr("vs", NULL), 1, ft_gputchar);
	tputs(tgetstr("ti", NULL), 1, ft_gputchar);
}

int		main(int argc, char **argv)
{
	prepare_tty();
	register_signals();
	if (!g_fd)
		return (1);
	ft_bzero(&g_ctx, sizeof(g_ctx));
	g_ctx.argc = (size_t)(argc - 1);
	g_ctx.argv = &argv[1];
	format_args();
	write_lines();
	read_input();
	restore_tty();
	free_args();
	return (0);
}
