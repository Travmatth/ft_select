/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/22 17:35:05 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	ft_select_exit(int status)
{
	exit(status);
}

void	ft_select_err(char *message)
{
	ft_putendl(message);
	exit(1);
}

void	restore_tty(void)
{
	if (ERR(tcsetattr(g_fd, TCSANOW, &g_tty)))
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
	g_fd = open(tty_id ? tty_id : "/dev/tty", O_RDWR);
	ft_printf("tty %s, fd %d\n", tty_id, g_log);
	g_log = getenv("FT_LOG") ? open(getenv("FT_LOG"), O_RDWR) : 2;
	ft_printf("here %s\n", getenv("FT_LOG"));
	if (!OK(g_fd) || !isatty(g_fd))
		ft_select_err("Not a terminal device");
	else if (NONE((type = getenv("TERM") ? getenv("TERM") : getenv("FTSHELL_TERM"))))
		ft_select_err("TERM or substitute not found");
	else if (ERR(tgetent(buf, type)))
		ft_select_err("Term type not valid");
	else if (ERR(tcgetattr(g_fd == STDOUT ? STDIN : g_fd, &g_tty)))
		ft_select_err("tcgetattr");
	ft_memcpy(&t, &g_tty, sizeof(struct termios));
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	// if (ERR(tcsetattr(g_fd == STDOUT ? STDIN : g_fd, TCSADRAIN, &t)))
	if (ERR(tcsetattr(0, TCSANOW, &t)))
		ft_select_err("tcsetattr");
	tputs(tgetstr("vs", NULL), 1, ft_gputchar);
	tputs(tgetstr("ti", NULL), 1, ft_gputchar);
}

int		main(int argc, char **argv)
{
	size_t	i;

	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigtstp_handler);
	signal(SIGCONT, sigcont_handler);
	signal(SIGWINCH, sigwinch_handler);
	prepare_tty();
	if (!g_fd)
		return (1);
	ft_bzero(&g_ctx, sizeof(g_ctx));
	g_ctx.argc = (size_t)(argc - 1);
	g_ctx.argv = &argv[1];
	format_args();
	display();
	restore_tty();
	i = 0;
	while (i < g_ctx.argc ? g_ctx.argc - 1 : 0)
	{
		if (g_ctx.selected[i])
		{
			write(g_fd, g_ctx.argv[i], g_ctx.lens[i]);
			write(g_fd, " ", 1);
		}
		i += 1;
	}
	free(g_ctx.selected);
	free(g_ctx.blanks);
	free(g_ctx.lens);
	return (0);
}
