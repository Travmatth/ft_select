/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/20 19:24:41 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_environ(int argc, char **argv)
{
	int		i;

	g_environ = (char**)ft_memalloc(sizeof(char*) * (argc + 1));
	i = 0;
	while (++i < argc)
		g_environ[i - 1] = ft_strdup(argv[i]);
}

int		main(int argc, char **argv)
{
	char	*command;

	init_environ(argc, argv);
	while (1)
	{
		signal(SIGINT, sig_handler);
		write(STDOUT, "$> ", 3);
		if (get_next_line(STDIN, &command) > 0)
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
