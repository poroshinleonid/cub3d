/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:25:31 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/25 16:27:46 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../libft/libft.h"
#include "../../MLX42/include/MLX42/MLX42.h"

int32_t get_color(t_data *data, int x, int y)
{
	(void)data;
	return (0xFF00FF - x + y);
}

void	drawscreen(void *ptr){
	t_data	*data = (t_data *)ptr;
	
	for(int i = 0; i < WIN_HEIGHT; i++)
	{
		for (int j = 0; j < WIN_WIDTH; j++)
		{
			mlx_put_pixel(data->mlx_img, i, j, get_color(data, i, j));
		}
	}
}

int	load_mlx_data(t_data *data)
{
	data->mlx_win = mlx_init(WIN_HEIGHT, WIN_WIDTH, "cub3d", 0);
	data->mlx_img = mlx_new_image(data->mlx_win, WIN_HEIGHT, WIN_WIDTH);
	mlx_image_to_window(data->mlx_win, data->mlx_img, 0, 0);
	mlx_loop_hook(data->mlx_win, drawscreen, data);
	return (0);
}