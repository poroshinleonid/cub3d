/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:25:31 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/27 15:49:26 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../libft/libft.h"
#include "../../MLX42/include/MLX42/MLX42.h"
#include "engine.h"
#include <math.h>

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

double	ft_abs(double num)
{
	if (num < 0)
		return (-num);
	return (num);
}

void	verLine(t_data *data, int x, int start, int end, uint32_t color)
{
	int i;

	printf("%d %d, %u\n", start, end, color);
	i = start;
	while (i <= end)
	{
		mlx_put_pixel(data->mlx_img, x, i, color);
		i++;
	}
}
//The vector is normalized by default!
void	ang_to_vec(double angle, t_vec *vector)
{
	vector->x = cos(angle);
	vector->y = sin(angle);
	vector->theta = angle;
	vector->triangle_angle = angle;
	while (vector->triangle_angle > PI/2 + EPS)
		vector->triangle_angle -= PI/2;
	return ;
}

t_vec	*get_next_point(t_data *data, t_vec ray_vec, t_vec cur_ray_pos, t_vec signs)
{
	double	hor_dx;
	double	hor_dy;
	double	ver_dx;
	double	ver_dy;
	double	hor_vectorlen;
	double	ver_vectorlen;
	t_vec	*res_vec;

	(void)data;
	res_vec = ft_calloc(sizeof(res_vec), 1);
	hor_dx = (int)(cur_ray_pos.x + (signs.x == 1)) - cur_ray_pos.x;
	hor_dy = hor_dx / tan(ray_vec.triangle_angle);
	hor_vectorlen = sqrt(hor_dx * hor_dx + hor_dy * hor_dy);


	ver_dy = (int)(cur_ray_pos.y + (signs.y == 1)) - cur_ray_pos.y;
	ver_dx = hor_dy / tan(PI/2 - ray_vec.triangle_angle);
	ver_vectorlen = sqrt(ver_dx * ver_dx + ver_dy * ver_dy);
	
	if (hor_vectorlen < ver_vectorlen)
	{
		res_vec->x = ray_vec.x + hor_dx;
		res_vec->y = ray_vec.y + hor_dy;
	}
	else
	{
		res_vec->x = ray_vec.x + ver_dx;
		res_vec->y = ray_vec.y + ver_dy;
	}
	return (res_vec);
}

int	closest_int(double f)
{
	int	i;

	i = (int)f;
	if (f - (double)i > 0.5)
		return (i + 1);
	return (i);
}

int	is_solid(t_data *data, t_vec *point)
{
	int	x;
	int	y;

	x = closest_int(point->x);
	y = closest_int(point->y);
	if ( x < 0 || x > data->map_w || y < 0 || y > data->map_h || data->map.grid[y][x] == 1)
		return (1);
	return(0);
}

double	distance(t_vec *v1, t_vec *v2)
{
	int	dx;
	int	dy;

	dx = v1->x - v2->x;
	dy = v1->y - v2->y;
	return (sqrt(dx * dx - dy * dy));
}

void	cast_one_ray(t_data *data, double cur_ang, int cur_x)
{
	t_vec	ray_vec;
	//t_vec	hor_vec;
	//t_vec	ver_vec;
	t_vec 	*next_intersection_v;
	int		hit;

	hit = 0;
	ang_to_vec(cur_ang, &ray_vec);
	t_vec	player_pos;
	player_pos.x = data->player.x;
	player_pos.y = data->player.y;
	player_pos.theta = data->player.theta;
	t_vec	cur_ray_pos;
	cur_ray_pos.x = data->player.x;
	cur_ray_pos.y = data->player.y;
	t_vec	signs;
	signs.x = (ray_vec.x > 0)?(1):(-1);
	signs.y = (ray_vec.y > 0)?(1):(-1);
	while (!hit)
	{
		//Infinite loop here for some reason!
		write(1, "S", 1);
		next_intersection_v = get_next_point(data, ray_vec, cur_ray_pos, signs);
		cur_ray_pos.x = next_intersection_v->x;
		cur_ray_pos.y = next_intersection_v->y;
		if (is_solid(data, next_intersection_v))
			hit = 1;
	}
	double	ray_len = distance(&player_pos, next_intersection_v);
	//height 1 -> 
	//int		height = ray_len * tan(FOV_ANGLE);
	int	wall_line_height = ray_len / WIN_HEIGHT;
	int	line_start = -wall_line_height / 2 + WIN_HEIGHT / 2;
	if (line_start < 0)
		line_start = 0;
	int	line_end = wall_line_height / 2 + WIN_HEIGHT / 2;
	if (line_end >= WIN_HEIGHT)
		line_end = WIN_HEIGHT - 1;
	verLine(data, cur_x, 0, line_start, 0x00000000);
	verLine(data, cur_x, line_start, line_end, 0xFFFFFFFF);
	verLine(data, cur_x, line_end, WIN_HEIGHT - 1, 0x00000000);
	//
	// Here we have next_intersection_v containing a point, and we know that this point is solid
	// Now we calculate the length of this vector,
	// from it determine the length of the line we need to draw on the screen
	// And normalize the line etc etc
	// And draw the line and floor and sky
}

//angles_amt = 120dg => 60 in each half
//ang_step = 120dg/WIN_HEIGHT
//min _ang = player_ang - [(angles_step * WIN_HEIGHT)/2 = (120dg/WIN_HEIGHT * WIN_HEIGHT)/2 = (2p/3)/2 = 2p/6]
//min_ang = player_ang - 2p/6
//max_ang = player_ang + 2p/6
void	cast_rays(t_data *data)
{
	double	cur_ang;
	double	ang_step;
	int		cur_x;

	cur_ang = data->player.theta - M_PI/3;
	ang_step = (2 * M_PI / 3)/WIN_WIDTH;
	cur_x = 0;
	while (cur_x < WIN_WIDTH)
	{
		cast_one_ray(data, cur_ang, cur_x);
		cur_ang += ang_step;
		cur_x++;
	}
}

void	drawscreen(void *ptr){
	t_data	*data = (t_data *)ptr;
	
	cast_rays(data);
	//draw_minimap(data);
}

void	listenkeys(mlx_key_data_t keydata, void* ptr)
{
	t_data *data;

	data = (t_data *)ptr;

	(void)keydata;
	mlx_t* mlx = data->mlx_win;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE) || mlx_is_key_down(mlx, MLX_KEY_Q))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		data->player.x -= PL_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		data->player.x += PL_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		data->player.y -= PL_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		data->player.y += PL_SPEED;
	return ;
}

int	load_mlx_data(t_data *data)
{
	data->mlx_win = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", 0);
	data->mlx_img = mlx_new_image(data->mlx_win, WIN_WIDTH, WIN_HEIGHT);
	mlx_image_to_window(data->mlx_win, data->mlx_img, 0, 0);
	//mlx_loop_hook(data->mlx_win, drawscreen, data);
	//mlx_key_hook(data->mlx_win, listenkeys,(void *)data);
	mlx_loop_hook(data->mlx_win, ft_hook, data);
	return (0);
}