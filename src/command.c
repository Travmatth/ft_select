/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/19 23:07:12 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int		find_exec(char *command)
{
	int			result;
	struct stat	attribs;

	result = access(command, X_OK);
	if (!result)
		return (1);
	if (!stat(command, &attribs))
	{
		if (!(attribs.st_mode & S_IXUSR))
			return (-1);
	}
	return (0);
}

int		find_command(char **command, char **paths, int i, int found)
{
	int		result;
	char	*tmp;
	char	*full_name;

	if ((result = find_exec(*command)) == -1 || result == 1)
		return (result);
	while (!found && --i >= 0)
	{
		tmp = ft_strjoin(paths[i], "/");
		full_name = ft_strjoin(tmp, *command);
		free(tmp);
		if ((result = find_exec(full_name)) == -1)
			found = 1;
		else if (result == 1)
		{
			found = 1;
			free(*command);
			*command = full_name;
		}
		else
			free(full_name);
	}
	return (result);
}

void	execute_command(char *command)
{
	char	**av;
	char	**paths;
	int		result;
	int		i;
	int		found;

	i = 0;
	found = 0;
	av = ft_strsplit(command, ' ');
	paths = ft_strsplit(get_env_var("PATH"), ':');
	signal(SIGINT, SIG_DFL);
	ft_printf("fork pid: %d\n", getpid());
	while (paths[i])
		i += 1;
	result = find_command(&av[0], paths, i, found);
	if (result)
		execve(av[0], av, g_environ);
	else if (result == -1)
		ft_printf("sh: permission denied: %s\n", av[0]);
	else
		ft_printf("sh: command not found: %s\n", av[0]);
	while (--i >= 0)
		free(paths[i]);
	while (av[++i])
		free(av[i]);
	g_processes -= 1;
	ft_printf("execute_command decrement: %d\n", g_processes);
}

void	execute_commands(char *command)
{
	char	**commands;
	int		i;
	pid_t	pid;

	i = -1;
	commands = ft_strsplit(command, ';');
	g_processes += 1;
	ft_printf("main pid: %d\n", getpid());
	ft_printf("execute_commands increment: %d\n", g_processes);
	while (commands[++i])
	{
		if (unbalanced_parentheses(commands[i]))
			ft_printf("please balance parentheses: %s\n", commands[i]);
		else if (builtin_command(commands[i]))
		{
			g_processes -= 1;
			ft_printf("builtin decrement: %d\n", g_processes);
		}
		else if (!(pid = fork()))
			execute_command(commands[i]);
		else
		{
			signal(SIGINT, child_sig_handler);
			waitpid(pid, NULL, 0);
			signal(SIGINT, sig_handler);
			ft_printf("waitpid decrement: %d\n", g_processes);
		}
	}
	while (i >= 0 && commands[i])
		free(commands[i--]);
}
