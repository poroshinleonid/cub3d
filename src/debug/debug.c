/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 18:36:46 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/24 19:20:16 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../libft/libft.h"

int	print_mapp(t_data *data)
{
	char	**lines;
	write(1, "\n", 1);
	ft_printf("NO: %s\n", data->map.textures.no);
	ft_printf("SO: %s\n", data->map.textures.so);
	ft_printf("EA: %s\n", data->map.textures.we);
	ft_printf("WE: %s\n", data->map.textures.ea);
	ft_printf(" F: %s\n", data->map.floor_col);
	ft_printf(" C: %s\n", data->map.sky_col);
	printf("Printed startn\n");
	lines = data->map.grid;
	while (*lines != NULL)
	{
		ft_printf("%s\n", lines[0]);
		lines++;
	}
	return (0);
}
