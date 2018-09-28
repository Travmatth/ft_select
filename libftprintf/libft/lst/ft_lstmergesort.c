/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmergesort.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/11 13:09:32 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/27 21:57:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// TODO
// verify operation and length
// implement for arr

static t_list	*merge(int *(*f)(void *first, void *second)
			, t_list *l
			, t_list *r
			, int reverse)
{
	t_list	*sorted;
	int		greater;

	sorted = NULL;
	while (ft_lstpeekhead(l) && ft_lstpeekhead(r))
	{
		greater = f(ft_lstpeekhead(l)->content, ft_lstpeekhead(r)->content);
		if (!reverse && greater)
		{
			ft_lstadd(&sorted, ft_lsthead(&l));
			ft_lstmerge(&sorted, ft_lsthead(&r));
		}
		else
		{
			ft_lstadd(&sorted, ft_lsthead(&r));
			ft_lstmerge(&sorted, ft_lsthead(&l));
		}
	}
	ft_lstmerge(&sorted, l);
	ft_lstmerge(&sorted, r);
	return (sorted);
}

static t_list	*lst_merge_sort(void *(*f)(void *first, void *second)
						, t_list **list, int reverse, size_t size)
{
	t_list	*left;
	t_list	*right;

	if (size > 1)
	{
		right = ft_lstsplit(list, size / 2);
		left = list;
		left = lst_merge_sort(f, left, reverse, size / 2);
		right = lst_merge_sort(f, right, reverse, ft_lstsize(right));
		return (merge(f, left, right, reverse));
	}
	else
		return (list);
}

void			*ft_lstmergesort(void *(*f)(void *first, void *second)
						, t_list **list, int reverse, size_t size)
{
	*list = lst_merge_sort(f, *list, reverse, ft_lstsize(list));
}
