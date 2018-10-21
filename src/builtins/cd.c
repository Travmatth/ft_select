/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 21:09:37 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/20 17:56:09 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		change_next_dir(char *dir, char **dirs, char *path, int i)
{
	struct stat	attribs;

	if (!chdir(dir))
		return (1);
	else if (access(dir, F_OK) == -1)
		ft_printf("cd: no such file or directory: %s\n", path);
	else if (access(dir, R_OK) == -1)
		ft_printf("cd: permission denied: %s\n", path);
	else if (!stat(path, &attribs) && !S_ISDIR(attribs.st_mode))
		ft_printf("not a directory: %s\n", path);
	else if (!dirs[i])
		ft_printf("%s\n", path);
	return (0);
}

void	change_dir(char *target_dir)
{
	char		*path;
	char		**dirs;
	char		*dir;
	int			i;

	i = 0;
	(target_dir[0] == '~' || ft_strequ("--", target_dir))
		? (path = ft_swap(target_dir, "~", get_env_var("HOME")))
		: (path = target_dir);
	dirs = ft_strsplit(path, '/');
	while ((dir = dirs[i++]))
	{
		if (change_next_dir(dir, dirs, path, i))
			continue ;
		else
			break ;
	}
	while (--i >= 0)
		free(dirs[i]);
	free(dirs);
	if (!ft_strequ(path, target_dir))
		free(path);
}

int		builtin_cd(char *command)
{
	char	**args;
	int		i;

	args = ft_strsplit(command, ' ');
	if (args[2])
		ft_printf("cd: string not in pwd: %s", args[1]);
	else if (args[1] && args[1][0] && args[1][0] != '.')
		change_dir(args[1]);
	else if (!args[1])
		change_dir("~");
	i = -1;
	while (args[++i])
		free(args[i]);
	free(args);
	return (1);
}
