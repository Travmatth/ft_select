/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 15:30:39 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/18 15:57:50 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		builtin_echo(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ft_printf("builtin_echo");
	return (1);
}

int		builtin_cd(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ft_printf("builtin_cd");
	return (1);
}

int		builtin_setenv(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ft_printf("builtin_setenv");
	return (1);
}

int		builtin_unsetenv(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ft_printf("builtin_unsetenv");
	return (1);
}

int		builtin_env(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ft_printf("builtin_env");
	return (1);
}
