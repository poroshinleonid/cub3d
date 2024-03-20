/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trusanov <trusanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:30:37 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 16:58:30 by trusanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42.h"
#include "config.h"
#include "cub3d.h"
#include "funcs.h"
#include "libft.h"
#include "textures.h"
#include <stdlib.h>

void	load_stuff(t_data *data)
{
	data->mlx = mlx_init(INIT_WIN_WIDTH, INIT_WIN_HEIGHT, "cub3d", 1);
	data->img_map = mlx_new_image(data->mlx, INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
	data->img_3d = mlx_new_image(data->mlx, INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
	data->img_width = INIT_WIN_WIDTH;
	data->img_height = INIT_WIN_HEIGHT;
	load_textures(data);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2 || !argv || !(argv[1]))
	{
		print_error("Can't parse command line arguments");
		return (EXIT_FAILURE);
	}
	if (parse_map(&data, argv[1]) == EXIT_FAILURE)
		terminate(&data, "Can't parse the map");
	load_stuff(&data);
	data.map.sky_col = data.map.sky_col * 0x100 + 0xFF;
	data.map.floor_col = data.map.floor_col * 0x100 + 0xFF;
	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop_hook(data.mlx, listen_keys, &data);
	mlx_close_hook(data.mlx, close_hook, &data);
	mlx_resize_hook(data.mlx, resize_hook, &data);
	mlx_image_to_window(data.mlx, data.img_3d, 0, 0);
	mlx_loop(data.mlx);
	return (EXIT_SUCCESS);
}
