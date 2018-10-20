/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 21:10:05 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/19 21:37:30 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		builtin_echo(char *command)
{
	char 	**args;
	int		i;
	int		skip;

	args = ft_strsplit(command, ' ');
	skip = 0;
	i = 1;
	if (args[1] && ft_strequ("-n", args[1]))
	{
		skip = 1;
		i = 2;
	}
	while (args[i])
	{
		ft_putstr(args[i++]);
		if (args[i])
			write(STDOUT, " ", 1);
	}
	while (--i >= 0)
		free(args[i]);
	free(args);
	if (!skip)
		write(STDOUT, "\n", 1);
	return (1);
}
