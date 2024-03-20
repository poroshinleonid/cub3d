/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:30:37 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 15:31:23 by lporoshi         ###   ########.fr       */
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
	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop_hook(data.mlx, listen_keys, &data);
	mlx_resize_hook(data.mlx, resize_hook, &data);
	mlx_image_to_window(data.mlx, data.img_3d, 0, 0);
	mlx_loop(data.mlx);
	return (EXIT_SUCCESS);
}
