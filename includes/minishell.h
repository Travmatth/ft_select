/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 15:06:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/29 18:04:09 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libftprintf/srcs/includes/ft_printf.h"
# include <unistd.h>
# include <signal.h>
# include <sys/stat.h>

# define IS_SEP(x) (x == ' ' || x == '\'' || x == '\t' || x== '"')

char			**g_environ;
int				g_processes;
typedef int		(*t_builtinf)(int argc, char **argv);
typedef struct	s_builtin
{
	char		*cmd;
	t_builtinf	f;
	int			len;
}				t_builtin;

/*
** parse.c
*/

int				remove_slash(char elem, size_t i, char *str, int *stop);
int				builtin_command(char **command);
int				prepare_command(char **commands, char ***command, int i);

/*
** quotes.c
*/

char			**remove_quotations(char *command, int ac);
int				count_params(char *command);

/*
** command.c
*/

void			expand_command(char **command);
int				execute_commands(char *command);

/*
** builtins.c
*/

void			set_env_var(char *name, char *val);
int				builtin_echo(int argc, char **argv);
int				builtin_cd(int argc, char **argv);
int				builtin_setenv(int argc, char **argv);
int				builtin_unsetenv(int argc, char **argv);
int				builtin_env(int argc, char **argv);

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
