/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:25:31 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/28 11:40:28 by lporoshi         ###   ########.fr       */
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
	if (num == -0.0)
		return (0.0);
	if (num < 0.000)
		return (num * -1.0 + 0.000000001);
	return (num);
}

void	verLine(t_data *data, int x, int start, int end, uint32_t color)
{
	int i;

	// printf("VerLine: x=%d; (%d, %d), %u\n", x, start, end, color);
	i = start;
	while (i <= end)
	{
		mlx_put_pixel(data->mlx_img, x, i, color);
		i++;
	}
}

void	render_ray(t_data *data, int x, int wall_start, int wall_end, uint32_t color)
{
	(void)color;
	verLine(data, x, 0, wall_start, 0xFFFF1F);
	verLine(data, x, wall_start, wall_end, 0xFF28F11F);
	verLine(data, x, wall_end, WIN_HEIGHT - 1, 0x11FFFF);
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
	// printf("gnp from (%f, %f); ray_vec = (%f, %f)\n", cur_ray_pos.x, cur_ray_pos.y, ray_vec.x, ray_vec.y);
	res_vec = ft_calloc(sizeof(res_vec), 1);
	if (ft_abs(ray_vec.x) > 0.999)
	{
		res_vec->x = cur_ray_pos.x + 1.0; 
		res_vec->y = cur_ray_pos.y;
		// printf("gnp RES (%f, %f); ray_vec = (%f, %f)\n", cur_ray_pos.x, cur_ray_pos.y, ray_vec.x, ray_vec.y);
		return(res_vec);
	}
	if (ray_vec.y > 0.999)
	{
		res_vec->x = cur_ray_pos.x;
		res_vec->y = cur_ray_pos.y + 1.0; 
		// printf("gnp RES (%f, %f); ray_vec = (%f, %f)\n", cur_ray_pos.x, cur_ray_pos.y, ray_vec.x, ray_vec.y);
		return(res_vec);
	}
	hor_dx = (cur_ray_pos.x + (signs.x == 1)) - cur_ray_pos.x;
	hor_dy = hor_dx / tan(ray_vec.triangle_angle);
	hor_vectorlen = sqrt(hor_dx * hor_dx + hor_dy * hor_dy);
	if (hor_vectorlen == 0)
		hor_vectorlen = INF;

	ver_dy = (cur_ray_pos.y + (signs.y == 1)) - cur_ray_pos.y;
	ver_dx = hor_dy / tan(PI/2 - ray_vec.triangle_angle);
	ver_vectorlen = sqrt(ver_dx * ver_dx + ver_dy * ver_dy);
	if (ver_vectorlen == 0)
		ver_vectorlen = INF;
	
	// printf("RAY CAST STEp INFO: hor_(%f, %f); ver(%f, %f) len(h%f, v%f)\n", \
	// hor_dx, hor_dy, ver_dx, ver_dy, hor_vectorlen, ver_vectorlen);
	if (hor_vectorlen == 0 && ver_vectorlen == 0)
	{
		// printf("Both are \n");
	}
	else if (hor_vectorlen < ver_vectorlen)
	{
		res_vec->x = cur_ray_pos.x + hor_dx;
		res_vec->y = cur_ray_pos.y + hor_dy;
		// printf("Movin HORIZ(x) (%f, %f)\n", res_vec->x,res_vec->y);
	}
	else 
	{
		res_vec->x = cur_ray_pos.x + ver_dx;
		res_vec->y = cur_ray_pos.y + ver_dy;
		// printf("Movin VER(Y) (%f, %f)\n", res_vec->x,res_vec->y);
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
	// printf("Checking if solid: (%lf, %lf)->(%d, %d), when map dimensions: %d, %d\n", \
	// point->x, point->y, x, y, \
	// data->map_w, data->map_h);

	// if(x <= 0 || x >= data->map_w - 1 || y <= 0 || y >= data->map_h - 1)
	// 	printf("Ray got out of the borders of the map !!!\n");
	if (x <= 0 || x >= data->map_w - 1 || y <= 0 || y >= data->map_h - 1 || data->map.grid[y][x] == 1)
		return (1);
	return(0);
}

double	distance(t_vec *v1, t_vec *v2)
{
	double	dx;
	double	dy;

	dx = v1->x - v2->x;
	dy = v1->y - v2->y;
	// printf("Calculating distance. (%f, %f) and (%f, %f)\n", v1->x, v1->y, v2->x, v2->y);
	// printf("dx and dy = %lf, %lf\n", dx, dy);
	return (sqrt(dx * dx + dy * dy));
}

t_vec	*first_collision(t_vec *dir_vector, t_vec start_pos)
{
	
}

void	cast_one_ray(t_data *data, double cur_ang, int cur_x)
{
	t_vec	ray_vec;
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
		// printf("rc_step from %f, %f; ray_vec = %f,%f\n",
			// cur_ray_pos.x, cur_ray_pos.y, ray_vec.x, ray_vec.y);
		next_intersection_v = get_next_point(data, ray_vec, cur_ray_pos, signs);
		cur_ray_pos.x = next_intersection_v->x;
		cur_ray_pos.y = next_intersection_v->y;
		if (is_solid(data, next_intersection_v))
			hit = 1;
	}
	double	ray_len = distance(&player_pos, next_intersection_v);
	//hypotenuse = ray_len
	//angle = angle
	//therefore, perpendicular distance = hypotenuse_len * sin(angle) or *cos(angle)
	// printf("Wrayy len1   = %f, cos = %f\n", ray_len, 110.0);
	// if(cos(cur_ang - data->player.theta) != 0.0)
	// 	ray_len = ft_abs(ray_len * ft_abs(cos(cur_ang - data->player.theta)));
	
	// printf("Wrayy len2   = %f\n", ray_len);
	//height 1 -> 
	//int		height = ray_len * tan(FOV_ANGLE);
	// int	wall_line_height = ((double)WIN_HEIGHT / ray_len) / 2.0;
	int	wall_line_height = ((double)WIN_HEIGHT / ray_len);
	// printf("wall line len   = %d\n", wall_line_height);
	int	line_start = (-wall_line_height / 2) + (WIN_HEIGHT / 2);
	if (line_start < 0)
		line_start = 0;
	int	line_end = (wall_line_height / 2) + (WIN_HEIGHT / 2);
	if (line_end >= WIN_HEIGHT)
		line_end = WIN_HEIGHT - 1;
	render_ray(data, cur_x, line_start, line_end, 0x00);
	// Here we have next_intersection_v containing a point, and we know that this point is solid
	// Now we calculate the length of this vector,
	// from it determine the length of the line we need to draw on the screen
	// And normalize the line etc etc
	// And draw the line and floor and sky
}

void	cast_rays(t_data *data)
{
	int		cur_x;
	
	cur_x = 0;
	data->ray.abs_ang = data->player.theta - FOV_HALFANGLE;
	while (cur_x < WIN_WIDTH - 10)
	{
		// ft_printf("casting ray on x=%d\n", cur_x);
		cast_one_ray(data, data->ray.abs_ang, cur_x);
		data->ray.abs_ang += ANGLE_STEP;
		cur_x++;
	}
	// ft_printf("all rays cast\n");
}

void	drawscreen(void *ptr){
	t_data	*data = (t_data *)ptr;
	
	cast_rays(data);
	// printf("Rendered screen!\n");
	//draw_minimap(data);
	mlx_image_to_window(data->mlx_win, data->mlx_img, 0, 0);
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