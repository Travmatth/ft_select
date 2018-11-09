/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 15:41:18 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/08 17:43:53 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include "../libftprintf/srcs/includes/ft_printf.h"
# include <termcap.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <termios.h>

typedef struct		s_offset
{
	size_t			*lens;
	short			*selected;
	size_t			width;
	int				rows;
	int				cols;
	int				focus;
}					t_offset;

typedef struct		s_tty
{
	struct termios	attr;
}					t_tty;

void				format_args(int argc
							, char **argv
							, t_offset *offsets);
#endif
