/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/20 19:24:49 by tmatthew         ###   ########.fr       */
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

void	expand_command(char **command)
{
	char	*var;
	char	*tmp;
	char	*cmd;

	while (ft_strchr(*command, '~'))
	{
		var = get_env_var("HOME");
		tmp = ft_swap(*command, "~", var);
		free(*command);
		*command = tmp;
	}
	while ((tmp = ft_strchr(*command, '$')))
	{
		cmd = ft_strchr(tmp, ' ') ? ft_strchr(tmp, ' ') : ft_strchr(tmp, '\0');
		tmp = ft_strsub(tmp, 0, cmd - tmp);
		var = get_env_var(tmp + 1);
		cmd = ft_swap(*command, tmp, var);
		free(*command);
		free(tmp);
		*command = cmd;
	}
}

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

int		unbalanced_parentheses(char *command)
{
	if (ft_count_char(command, '"') % 2)
		return (1);
	else if (ft_count_char(command, '\'') % 2)
		return (1);
	return (0);
}
