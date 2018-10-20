/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/19 22:58:01 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_builtin	g_builtins[] =
{
	{"echo", builtin_echo, 4},
	{"cd", builtin_cd, 2},
	{"setenv", builtin_setenv, 6},
	{"unsetenv", builtin_unsetenv, 8},
	{"env", builtin_env, 3},
};

int		builtin_command(char *command)
{
	int		i;
	int		j;

	i = 0;
	while (i < 5)
	{
		if (ft_strnequ(g_builtins[i].cmd, command, g_builtins[i].len))
		{
			j = 0;
			return (g_builtins[i].f(command));
		}
		i += 1;
	}
	return (0);
}

void	init_environ(int argc, char **argv)
{
	int		i;

	g_environ = (char**)ft_memalloc(sizeof(char*) * (argc + 1));
	i = 0;
	while (++i < argc)
		g_environ[i - 1] = ft_strdup(argv[i]);
}

int		unbalanced_parentheses(char *command)
{
	if (ft_count_char(command, '"') % 2)
		return (1);
	else if (ft_count_char(command, '\'') % 2)
		return (1);
	return (0);
}

int		main(int argc, char **argv)
{
	char	*command;

	init_environ(argc, argv);
	signal(SIGINT, sig_handler);
	signal(SIGCHLD, child_sig_handler);
	while (1)
	{
		ft_printf("processes: %d, pid: %d ", g_processes, getpid());
		write(STDOUT, "$> ", 3);
		get_next_line(STDIN, &command);
		if (command)
		{
			expand_command(&command);
			if (ft_strequ("exit", command))
				break ;
			execute_commands(command);
		}
		else
			break ;
	}
	return (0);
}
