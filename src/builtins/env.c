/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 21:10:34 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/20 18:07:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		builtin_env(char *command)
{
	int		i;

	(void)command;
	i = 0;
	while (g_environ[i])
		ft_printf("%s\n", g_environ[i++]);
	return (1);
}
