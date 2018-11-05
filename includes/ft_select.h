/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 15:41:18 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/04 20:31:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include "../libftprintf/srcs/includes/ft_printf.h"
# include <termcap.h>
# include <sys/ioctl.h>

typedef struct	s_args
{
	int			max;
	int			total;
}				t_args;

typedef struct	s_opt
{
	size_t		start;
	size_t		len;
}				t_opt;

typedef struct	s_offset
{
	int			rows;
	int			cols;
	size_t		*start;
	size_t		*len;
}				t_offset;
#endif
