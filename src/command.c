/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/20 19:24:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	execute_command(char **av)
{
	pid_t	pid;

	if (!(pid = fork()))
		execve(av[0], av, g_environ);
	else if (pid > 0)
	{
		ft_printf("parent pid: %d\n", getpid());
		waitpid(pid, NULL, 0);
	}
}

void	execute_cmd(char **commands)
{
	int		j;
	int		found;
	char	**av;
	char	**paths;
	int		result;

	j = 0;
	found = 0;
	av = ft_strsplit(commands[j], ' ');
	paths = ft_strsplit(get_env_var("PATH"), ':');
	while (paths[j])
		j += 1;
	result = find_command(&av[0], paths, j, found);
	if (result)
		execute_command(av);
	else if (result == -1)
		ft_printf("sh: permission denied: %s\n", av[0]);
	else
		ft_printf("sh: command not found: %s\n", av[0]);
	while (--j >= 0)
		free(paths[j]);
	while (av[++j])
		free(av[j]);
}

void	execute_commands(char *command)
{
	char	**commands;
	int		i;

	i = -1;
	commands = ft_strsplit(command, ';');
	while (commands[++i])
	{
		g_processes += 1;
		if (unbalanced_parentheses(commands[i]))
			ft_printf("please balance parentheses: %s\n", commands[i]);
		else if (builtin_command(commands[i]))
			;
		else
			execute_cmd(commands);
		g_processes -= 1;
	}
	while (i >= 0 && commands[i])
		free(commands[i--]);
}
