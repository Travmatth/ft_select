/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   widths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 18:09:50 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/09 14:53:20 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void	*find_link(void *final, t_list *elem, size_t i, int *stop)
{
	t_dir	*d;
	int		x;

	(void)i;
	(void)stop;
	d = (t_dir*)elem->content;
	x = (int)LEN(d->links, 0);
	if (!final)
		return (ft_memdup(&x, sizeof(int)));
	else if (x > *(int*)final)
	{
		free(final);
		return (ft_memdup(&x, sizeof(int)));
	}
	return (final);
}

void	*find_name(void *final, t_list *elem, size_t i, int *stop)
{
	t_dir	*d;
	int		x;

	(void)i;
	(void)stop;
	d = (t_dir*)elem->content;
	x = (int)LEN(d->owner_name, 0);
	if (!final)
		return (ft_memdup(&x, sizeof(int)));
	else if (x > *(int*)final)
	{
		free(final);
		return (ft_memdup(&x, sizeof(int)));
	}
	return (final);
}

void	*find_grp(void *final, t_list *elem, size_t i, int *stop)
{
	t_dir	*d;
	int		x;

	(void)i;
	(void)stop;
	d = (t_dir*)elem->content;
	x = (int)LEN(d->owner_group, 0);
	if (!final)
		return (ft_memdup(&x, sizeof(int)));
	else if (x > *(int*)final)
	{
		free(final);
		return (ft_memdup(&x, sizeof(int)));
	}
	return (final);
}

void	*find_size(void *final, t_list *elem, size_t i, int *stop)
{
	t_dir	*d;
	int		x;

	(void)i;
	(void)stop;
	d = (t_dir*)elem->content;
	x = (int)LEN(d->size, 0);
	if (!final)
		return (ft_memdup(&x, sizeof(int)));
	else if (x > *(int*)final)
	{
		free(final);
		return (ft_memdup(&x, sizeof(int)));
	}
	return (final);
}

int		*find_widths(t_list *lst, int widths[4])
{
	int		i;
	int		*tmp;

	i = 0;
	if (!lst)
		ft_bzero((void*)widths, sizeof(int) * 4);
	else
	{
		tmp = ft_lstfoldl(find_link, lst);
		widths[0] = *tmp;
		free(tmp);
		tmp = ft_lstfoldl(find_name, lst);
		widths[1] = *tmp;
		free(tmp);
		tmp = ft_lstfoldl(find_grp, lst);
		widths[2] = *tmp;
		free(tmp);
		tmp = ft_lstfoldl(find_size, lst);
		widths[3] = *tmp;
		free(tmp);
	}
	return (widths);
}
