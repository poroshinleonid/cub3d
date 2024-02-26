/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:25:31 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/26 14:07:41 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../libft/libft.h"
#include "../../MLX42/include/MLX42/MLX42.h"

int32_t get_color(t_data *data, int x, int y)
{
	(void)data;
	return (x * y);
}

int32_t get_color_minimap(t_data *data)
{
	int	x;
	int	y;

	x = data->rect.mapx;
	y = data->rect.mapy;
	if (x < 0 || y < 0 || x >= data->map_w|| y >= data->map_h)
		return (0xFFFF00);
	if (data->map.grid[y][x] == '1')
		return (0xFF0000FF);
	if (data->map.grid[y][x] == '0' || ft_in(data->map.grid[y][x], MAP_PLAYER_CHARSET))
		return (0xFFFFFFFF);
	return (0xFFFF00);
}

void	draw_rect(t_data *data, int x, int y)
{
	int	tmpx;
	int32_t	cur_col;

	printf("Drawing (%d, %d) to (%d, %d)\n", x, y, data->rect.dx, data->rect.dy);
	cur_col = get_color_minimap(data);
	tmpx = x;
	while (y < data->rect.dy)
	{
		x = tmpx;
		while (x < data->rect.dx)
		{
			mlx_put_pixel(data->mlx_img, x, y, cur_col);
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < 21)
	{
		x = 0;
		while (x < 21)
		{
			data->rect.mapx = x;
			data->rect.mapy = y;
			data->rect.dx = x * TILE_SZ + TILE_SZ - TILE_BORDER_SZ;
			data->rect.dy = y * TILE_SZ + TILE_SZ - TILE_BORDER_SZ;
			draw_rect(data, x  * TILE_SZ + TILE_BORDER_SZ, y * TILE_SZ + TILE_BORDER_SZ);
			x++;
		}
		y++;
	}
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
	draw_minimap(data);
}

int	load_mlx_data(t_data *data)
{
	data->mlx_win = mlx_init(WIN_HEIGHT, WIN_WIDTH, "cub3d", 0);
	data->mlx_img = mlx_new_image(data->mlx_win, WIN_HEIGHT, WIN_WIDTH);
	mlx_image_to_window(data->mlx_win, data->mlx_img, 0, 0);
	mlx_loop_hook(data->mlx_win, drawscreen, data);
	return (0);
}