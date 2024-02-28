/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:25:31 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/28 12:42:42 by lporoshi         ###   ########.fr       */
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

void	calc_triangle_angle(t_vec *vector)
{
	vector->triangle_angle = vector->theta;
	while (vector->triangle_angle > PI/2 + EPS)
		vector->triangle_angle -= PI/2;
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

double	vec_len(t_vec *vector)
{
	return (sqrt((vector->x * vector->x) + (vector->y * vector->y)));
}

double	ft_modf(double d)
{
	double	tmp;
	double	res;

	res = modf(d, &tmp);
	return (res);
}

void	calc_first_collisions(t_data *data)
{
	if ((data->ray.abs_ang > M_PI - EPS && data->ray.abs_ang < M_PI + EPS) || \
		((data->ray.abs_ang < EPS && data->ray.abs_ang > -EPS)))
		data->ray.x_step_vec.x = 0;
	else if (data->ray.abs_ang > PI1_2 && data->ray.abs_ang < PI3_2)
		data->ray.x_step_vec.x =  ft_modf(data->player.x);
	else 
		data->ray.x_step_vec.x = 1 - ft_modf(data->player.x);
	data->ray.x_step_vec.y = data->ray.x_step_vec.x * cos(data->ray.abs_ang);
	if ((data->ray.abs_ang > PI1_2 - EPS && data->ray.abs_ang < PI1_2 + EPS) || \
		((data->ray.abs_ang < PI3_2 + EPS && data->ray.abs_ang >PI3_2 - EPS)))
		data->ray.x_step_vec.y = 0;
	else if (data->ray.abs_ang > PI1_2 && data->ray.abs_ang < PI3_2)
		data->ray.y_step_vec.y =  ft_modf(data->player.y);
	else 
		data->ray.y_step_vec.y = 1 - ft_modf(data->player.y);
	data->ray.y_step_vec.x = data->ray.y_step_vec.y * cos(data->ray.abs_ang);
	if (vec_len(&(data->ray.x_step_vec)) < vec_len(&(data->ray.y_step_vec)))
	{
		data->ray.cur_pos.x = data->ray.x_step_vec.x;
		data->ray.cur_pos.y = data->ray.x_step_vec.y;
		return ;
	}
	data->ray.cur_pos.x = data->ray.y_step_vec.x;
	data->ray.cur_pos.y = data->ray.y_step_vec.y;
}

void	calc_step_lengths(t_data *data)
{
	//we have data->ray.cur_pos for the current position.
	//we have angle so we can calculate using cos() or something,
	//what will dx/dy be for both of the casts
	data->ray.x_step_vec.x = data->ray.dir_vec.x;
	data->ray.x_step_vec.y = tan(data->ray.dir_vec.triangle_angle) * data->ray.dir_vec.x;
	data->ray.y_step_vec.y = data->ray.dir_vec.y;
	data->ray.y_step_vec.x = tan(data->ray.dir_vec.triangle_angle) * data->ray.dir_vec.y;
}

void	save_quarter(t_data *data)
{
	double	a;

	a = data->ray.abs_ang;
	if (a > 0.0 && a < M_PI)
		data->ray.dir_vec.y = -1;
	else
		data->ray.dir_vec.y = 1;
	if (a > 0.0 && a < M_PI)
		data->ray.dir_vec.x = -1;
	else
		data->ray.dir_vec.x = 1;
	data->ray.dir_vec.theta = data->ray.abs_ang;
	calc_triangle_angle(&(data->ray.dir_vec));
}

void	cast_one_ray(t_data *data)
{
	save_quarter(data);
	calc_first_collisions(data);
	calc_step_lengths(data);
	while (!is_solid(data, &(data->ray.cur_pos)))
	{
		// aaaaaa
		// do we need to store xray and yray separately?
		//Do we increment them in one cycle idk aaa
	}
	//get_line_height(data);
	//render_ray(data, cur_x, data->ray.wall_top, data->ray.wall_bot, 0x00);
}

void	cast_rays(t_data *data)
{
	int		cur_x;
	
	cur_x = 0;
	data->ray.abs_ang = data->player.theta - FOV_HALFANGLE;
	data->ray.rel_ang = -FOV_HALFANGLE;
	while (cur_x < WIN_WIDTH - 10)
	{
		cast_one_ray(data);
		data->ray.abs_ang += ANGLE_STEP;
		data->ray.rel_ang += ANGLE_STEP;
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