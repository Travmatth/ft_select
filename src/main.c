/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/13 17:57:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_select.h"

void	ft_select_err(char *message)
{
	ft_putendl(message);
	exit(1);
}

void	prepare_tty(t_tty *term)
{
	char			*tty_id;
	char			*type;
	char			buf[BUFF_SIZE];
	struct termios	t;

	tty_id = getenv("FTSHELL_TTY");
	g_fd = tty_id ? open(tty_id, O_RDWR) : STDOUT;
	if (!OK(g_fd) || !isatty(g_fd))
		ft_select_err("Not a terminal device");
	else if (NONE((type = getenv(g_fd == 1 ? "TERM" : "FTSHELL_TERM"))))
		ft_select_err("TERM or substitute not found");
	else if (ERR(tgetent(buf, type)))
		ft_select_err("Term type not valid");
	else if (ERR(tcgetattr(g_fd, &term->attr)))
		ft_select_err("tcgetattr");
	ft_memcpy(&t, &term->attr, sizeof(struct termios));
	t = term->attr;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (ERR(tcsetattr(g_fd, TCSADRAIN, &t)))
		ft_select_err("tcsetattr");
	tputs(tgetstr("vs", NULL), 1, ft_gputchar);
	tputs(tgetstr("ti", NULL), 1, ft_gputchar);
}

void	write_choices(int argc, char **argv, t_ctx *ctx)
{
	int		i;

	i = 0;
	while (i < argc)
	{
		if (ctx->selected[i])
		{
			write(g_fd, argv[i], ctx->lens[i]);
			write(g_fd, " ", 1);
		}
		i += 1;
	}
}

int		main(int argc, char **argv)
{
	t_tty		tty;
	t_ctx	ctx;

	prepare_tty(&tty);
	if (!g_fd)
		return (1);
	ft_bzero(&ctx, sizeof(ctx));
	format_args(argc - 1, &argv[1], &ctx);
	display(argc - 1, &argv[1], &ctx);
	if (ERR(tcsetattr(g_fd, TCSANOW, &tty.attr)))
		ft_select_err("tcsetattr");
	tputs(tgetstr("ve", NULL), 1, ft_gputchar);
	tputs(tgetstr("te", NULL), 1, ft_gputchar);
	write_choices(argc - 1, &argv[1], &ctx);
	return (0);
}
