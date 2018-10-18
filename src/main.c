/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/18 16:01:03 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_builtin	g_builtins[] =
{
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"setenv", builtin_setenv},
	{"unsetenv", builtin_unsetenv},
	{"env", builtin_env},
};

int		builtin_command(char *command)
{
	int		found;
	int		i;
	int		arg_c;
	char	**arg_v;

	i = 0;
	found = 0;
	while (i < 5)
	{
		if (ft_strequ(g_builtins[i].cmd, command))
		{
			i = 0;
			arg_c = 0;
			arg_v = ft_strsplit(command, ' ');
			while (arg_v[i++])
				arg_c += 1;
			arg_c += 1;
			return g_builtins[i].f(arg_c, arg_v);
		}
		i += 1;
	}
	return (found);
}

int		main(int argc, char **argv)
{
	pid_t	child;
	char	*command;

	(void)argc;
	(void)argv;
	while (1)
	{
		get_next_line(STDIN, &command);
		if (ft_strequ("exit", command))
			break ;
		else if (builtin_command(command))
			continue ;
		else if (!(child = fork()))
		{
			ft_printf("child: %s", command);
		}
		else
		{
			ft_printf("parent: %s", command);
		}
	}
	return (0);
}
