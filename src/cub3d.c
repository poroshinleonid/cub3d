/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:53:15 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/24 19:12:19 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"
#include "../../libft/libft.h"

int	main(int argc, char **argv)
{
	t_data	data;
	(void)argc;
	(void)argv;
	char	**tests;

	tests = ft_split("NO ./path_to_the_north_texture", ' ');
	ft_printf("! [%s]\n", tests[0]);
	ft_printf("! [%s]\n", tests[1]);
	ft_printf("! [%s]\n", tests[2]);


	ft_printf("RUNNING:\n");
	parse_map(&data, argv[1]);
	print_mapp(&data);
	
}
