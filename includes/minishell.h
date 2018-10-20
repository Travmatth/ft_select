/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 15:06:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/19 22:08:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libftprintf/srcs/includes/ft_printf.h"
# include <unistd.h>
# include <signal.h>
# include <sys/stat.h>

char			**g_environ;
int				g_processes;
typedef int		(*t_builtinf)(char *command);
typedef struct	s_builtin
{
	char		*cmd;
	t_builtinf	f;
	int			len;
}				t_builtin;

/*
** main.c
*/

int				builtin_command(char *command);
int				unbalanced_parentheses(char *command);

/*
** command.c
*/

void			expand_command(char **command);
void			execute_commands(char *command);

/*
** builtins.c
*/

int				builtin_echo(char *command);
int				builtin_cd(char *command);
int				builtin_setenv(char *command);
int				builtin_unsetenv(char *command);
int				builtin_env(char *command);

/*
** env.c
*/

char			*get_env_var(char *var);

/*
** signal.c
*/

void			child_sig_handler(int sig);
void			sig_handler(int sig);
#endif
