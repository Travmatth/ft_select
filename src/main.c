/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/27 17:12:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	restore_tty(void)
{
	g_tty.c_lflag &= (ICANON | ECHO | ISIG | IEXTEN | ECHOE | ECHOK | ECHOKE
		| ECHOCTL);
	if (ERR(tcsetattr(g_fd, TCSADRAIN, &g_tty)))
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
	g_fd = open(tty_id ? tty_id : ttyname(STDIN), O_RDWR);
	if (!OK(g_fd) || !isatty(g_fd))
		ft_select_err("Not a terminal device");
	else if (NONE((type = getenv("TERM")
		? getenv("TERM") : getenv("FTSHELL_TERM"))))
		ft_select_err("TERM or substitute not found");
	else if (ERR(tgetent(buf, type)))
		ft_select_err("Term type not valid");
	else if (ERR(tcgetattr(g_fd, &g_tty)))
		ft_select_err("tcgetattr");
	else if (ERR(tcgetattr(g_fd, &t)))
		ft_select_err("tcgetattr");
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (ERR(tcsetattr(g_fd, TCSANOW, &t)))
		ft_select_err("tcsetattr");
	tputs(tgetstr("ti", NULL), 1, ft_gputchar);
	tputs(tgetstr("vi", NULL), 1, ft_gputchar);
}

int		main(int argc, char **argv)
{
	prepare_tty();
	register_signals();
	ft_bzero(&g_ctx, sizeof(g_ctx));
	g_ctx.argc = (size_t)(argc - 1);
	g_ctx.argv = &argv[1];
	format_args();
	write_lines();
	read_input();
	restore_tty();
	write_args();
	return (0);
}
