/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 21:10:34 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/24 15:27:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		builtin_env(int argc, char **argv)
{
	int		i;

	(void)argc;
	(void)argv;
	i = 0;
	while (g_environ[i])
		ft_printf("%s\n", g_environ[i++]);
	return (1);
}
