/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:08:47 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/23 20:31:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <fcntl.h>
# include "../libftprintf/srcs/includes/ft_printf.h"

# define SET_LONG(v) (BITSET(v, 0))
# define SET_RECURSE(v) (BITSET(v, 1))
# define SET_ALL(v) (BITSET(v, 2))
# define SET_REVERSE(v) (BITSET(v, 3))
# define SET_SORT_TIME(v) (BITSET(v, 4))
# define SET_SORT_ACCESS(v) (BITSET(v, 5))
# define SET_NO_SORT(v) (BITSET(v, 6))
# define SET_DISPL_GRP(v) (BITSET(v, 7))
# define SET_NO_RECURSE(v) (BITSET(v, 8))
# define SET_COLOR_OUT(v) (BITSET(v, 9))

# define GET_LONG(v) (BITSET(v, 0))
# define GET_RECURSE(v) (BITSET(v, 1))
# define GET_ALL(v) (BITSET(v, 2))
# define GET_REVERSE(v) (BITSET(v, 3))
# define GET_SORT_TIME(v) (BITSET(v, 4))
# define GET_SORT_ACCESS(v) (BITSET(v, 5))
# define GET_NO_SORT(v) (BITSET(v, 6))
# define GET_DISPL_GRP(v) (BITSET(v, 7))
# define GET_NO_RECURSE(v) (BITSET(v, 8))
# define GET_COLOR_OUT(v) (BITSET(v, 9))

typedef struct	s_ls
{
	char flags[BITNSLOTS(10)];
}

#endif
