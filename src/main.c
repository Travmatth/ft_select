/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/23 20:44:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int		main(int argc, char **argv)
{
	t_ls	ctx;

	if (argc == 1)
		ft_ls_usage(NULL);
	else if (parse_opts(&ctx, argc, argv))
	{
		crawl_files(ctx, ctx->base);
		print_files(ctx);
	}
	else
		ft_ls_usage(argv);
	return (0);
}
