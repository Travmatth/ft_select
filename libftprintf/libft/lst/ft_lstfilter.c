/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfilter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 21:00:41 by tmatthew          #+#    #+#             */
/*   Updated: 2018/05/19 18:28:50 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
** ft_lstfilter
** Description Iterates the list lst and applies the function f to each link.
** Param. #1 A pointer to the first link of a list.
** Param. #2 The address of a function to apply to each link of a list.
** Return value None.
** Libc functions None.
*/

t_list	*take_top(t_list *lst
	, int (*f)(t_list *elem)
	, void (*d)(t_list *elem))
{
	if (f(lst))
	{
		d(lst);
		free(lst->content);
		free(lst);
		return (NULL);
	}
	else
		return (lst);
}

t_list	*filter_lists(t_list *lst
	, int (*f)(t_list *elem)
	, void (*d)(t_list *elem))
{
	t_list	*parent;
	t_list	*tmp;

	tmp = lst;
	parent = tmp;
	while (tmp)
	{
		if (f(tmp))
		{
			parent->next = tmp->next;
			d(tmp);
			free(tmp->content);
			free(tmp);
			tmp = parent;
		}
		parent = tmp;
		tmp = tmp->next;
	}
	return (lst);
}

t_list	*ft_lstfilter(t_list *lst
	, int (*f)(t_list *elem)
	, void (*d)(t_list *elem))
{
	if (!lst)
		return (NULL);
	if (!lst->next)
		return (take_top(lst, f, d));
	return (filter_lists(lst, f, d));
}
