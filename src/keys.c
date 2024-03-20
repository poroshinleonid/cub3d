/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trusanov <trusanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:30:32 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 17:25:55 by trusanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42.h"
#include "config.h"
#include "cub3d.h"
#include "funcs.h"
#include "libft.h"
#include <math.h>
#include <stdlib.h>

static void	rotation_hook(t_data *data)
{
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
	{
		data->player.a += PL_ROT_SPEED;
		if (data->player.a > 2 * PI)
			data->player.a -= 2 * PI;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
	{
		data->player.a -= PL_ROT_SPEED;
		if (data->player.a < 0.0)
			data->player.a += 2 * PI;
	}
	data->player.dx = cos(data->player.a);
	data->player.dy = sin(data->player.a);
}

void	listen_keys(void *void_data)
{
	t_data	*data;

	data = (t_data *)void_data;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE) \
		|| mlx_is_key_down(data->mlx, MLX_KEY_Q))
	{
		free_mem(data);
		ft_printf("Goodbye!\n");
		exit(0);
	}
	movement_hook(data);
	rotation_hook(data);
}

void	resize_hook(int32_t width, int32_t height, void *void_data)
{
	t_data	*data;

	data = (t_data *)void_data;
	data->img_width = width;
	data->img_height = height;
	mlx_resize_image(data->img_3d, width, height);
}

void	close_hook(void *void_data)
{
	t_data	*data;

	data = (t_data *)void_data;
	free_mem(data);
	ft_printf("Goodbye!\n");
	exit(0);
}
