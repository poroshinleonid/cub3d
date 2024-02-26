/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 18:36:46 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/26 13:14:23 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../libft/libft.h"
#include <stdio.h>

int	print_mapp(t_data *data)
{
	char	**lines;
	write(1, "\n", 1);
	ft_printf("NO: %s\n", data->map.textures.no_path);
	ft_printf("SO: %s\n", data->map.textures.so_path);
	ft_printf("EA: %s\n", data->map.textures.we_path);
	ft_printf("WE: %s\n", data->map.textures.ea_path);
	ft_printf(" F: %d\n", data->map.floor_col);
	ft_printf(" C: %d\n", data->map.sky_col);
	printf("Player: (%f, %f), theta=%f\n", data->player.x, data->player.y, data->player.theta);
	lines = data->map.grid;
	while (lines && *lines != NULL)
	{
		ft_printf("%s\n", lines[0]);
		lines++;
	}
	return (0);
}
