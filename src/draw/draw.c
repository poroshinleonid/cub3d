/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:25:31 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/29 14:30:56 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../libft/libft.h"
#include "../../MLX42/include/MLX42/MLX42.h"
#include "engine.h"
#include <math.h>

void	vec_copy(t_vec src, t_vec *dst)
{
	dst->x = src.x;
	dst->y = src.y;
	dst->theta = src.theta;
	dst->triangle_angle = src.triangle_angle;
}

double	distance(t_vec *v1, t_vec *v2)
{
	double	dx;
	double	dy;

	dx = v1->x - v2->x;
	dy = v1->y - v2->y;
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
//ASSUMIG ANGLES ARE THE SAME!!!
void	vec_write_sum_parallel(t_vec *dst, t_vec arg1, t_vec arg2)
{
	dst->x = arg1.x + arg2.x;
	dst->y = arg1.y + arg2.y;
}

void	normalize_vec_to_map(t_data *data, t_vec *vec)
{
	if (vec->x < 1)
		vec->x = 1;
	if (vec->x >= data->map_w)
		vec->x = data->map_w - 1;
	if (vec->y < 1)
		vec->y = 1;
	if (vec->y >= data->map_h)
		vec->y = data->map_h - 2;
}

void	ray_step(t_data *data)
{
	t_vec	next_x_ray;
	t_vec	next_y_ray;
	double	dist_x;
	double	dist_y;
	
	//
	vec_write_sum_parallel(&next_x_ray, data->ray.cur_x_ray, data->ray.x_step_vec);
	vec_write_sum_parallel(&next_y_ray, data->ray.cur_y_ray, data->ray.y_step_vec);
	normalize_vec_to_map(data, &next_x_ray);
	normalize_vec_to_map(data, &next_y_ray);
	dist_x = distance(&(data->ray.player_pos), &next_x_ray);
	dist_y = distance(&(data->ray.player_pos), &next_y_ray);
	if (dist_x < dist_y)
	{
		vec_copy(next_x_ray, &(data->ray.cur_pos));
		vec_copy(next_x_ray, &(data->ray.cur_x_ray));
	}
	else
	{
		vec_copy(next_y_ray, &(data->ray.cur_pos));
		vec_copy(next_y_ray, &(data->ray.cur_y_ray));
	}
}

void draw_line(t_data *data, int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx, sy;

    if (x1 < x2) {
        sx = 1;
    } else {
        sx = -1;
    }

    if (y1 < y2) {
        sy = 1;
    } else {
        sy = -1;
    }

    int err = dx - dy;

    while (1) {
		if (x1 < 0 || x1 >= WIN_WIDTH || y1 < 0 || y1 >= WIN_HEIGHT)
			return ;
		mlx_put_pixel(data->mlx_img, x1, y1, color);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}



void draw_line_c(t_data *data, int start_x, int start_y, int length, double angle_rad, int color) {
    //double angle_rad = line_angle * M_PI / 180.0; // Convert angle to radians
    int end_x = start_x + length * cos(angle_rad);
    int end_y = start_y + length * sin(angle_rad);

    int dx = abs(end_x - start_x);
    int dy = abs(end_y - start_y);
    int sx, sy;

    if (start_x < end_x) {
        sx = 1;
    } else {
        sx = -1;
    }

    if (start_y < end_y) {
        sy = 1;
    } else {
        sy = -1;
    }

    int err = dx - dy;

    while (1) {
        if (start_x < 0 || start_x >= WIN_WIDTH || start_y < 0 || start_y >= WIN_WIDTH)
				return ;
		mlx_put_pixel(data->mlx_img, start_x, start_y, color);

        if (start_x == end_x && start_y == end_y) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            start_x += sx;
        }
        if (e2 < dx) {
            err += dx;
            start_y += sy;
        }
    }
}
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
			if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_WIDTH)
				return ;
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
	while (y < 6)
	{
		x = 0;
		while (x < 6)
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

void	calculate_map_block(t_data *data, t_vec *point, int *x, int *y)
{
	(void)data;
	(void)point;
	(void)x;
	(void)y;
	
	return ;
}

int	is_solid(t_data *data, t_vec *point)
{
	int	x;
	int	y;
	return (1);
	// x = closest_int(point->x);
	// y = closest_int(point->y);
	x = (int)point->x;
	y = (int)point->y;
	//calculate_map_block(data, point, &x, &y);
	printf("Checking if solid: (%lf, %lf)->(%d, %d), when map dimensions: %d, %d\n", \
	point->x, point->y, x, y, \
	data->map_w, data->map_h);

	// if(x <= 0 || x >= data->map_w - 1 || y <= 0 || y >= data->map_h - 1)
	// 	printf("Ray got out of the borders of the map !!!\n");
	if (x <= 1 || x >= data->map_w - 1 || y <= 1 || y >= data->map_h - 1 || data->map.grid[y][x] == 1)
		return (1);
	return(0);
}


double	get_dx(t_data *data)
{
	if (data->ray.dir_vec.x > 0)
		return (1 - ft_modf(data->player.x));
	else
		return (-ft_modf(data->player.x));
}

double	get_dy(t_data *data)
{
	if (data->ray.dir_vec.y > 0)
		return (1 - ft_modf(data->player.y));
	else
		return (-ft_modf(data->player.y));
}

int	ft_double_eq(double a, double b)
{
	if (a - b > -EPS && a - b < EPS)
		return (1);
	return (0);
}

void	calc_step_lengths(t_data *data)
{
	printf("dir_vecs: x[%f] y[%f]\n", data->ray.dir_vec.x, data->ray.dir_vec.y);
	printf("triangle: (%f)\n", data->ray.tr_ang);
	
	data->ray.x_step_vec.x = data->ray.dir_vec.x;
	if (ft_double_eq(data->ray.tr_ang, PI1_2))
		data->ray.x_step_vec.y = 0;
	else
		data->ray.x_step_vec.y = tan(ft_abs(data->ray.tr_ang)) * data->ray.dir_vec.y;
	
	
	data->ray.y_step_vec.y = data->ray.dir_vec.y;
	if (ft_double_eq(data->ray.tr_ang, PI1_2))
		data->ray.y_step_vec.x = 0;
	else
		data->ray.y_step_vec.x = tan(ft_abs(data->ray.tr_ang)) * data->ray.dir_vec.x;
	printf("Step_lengths: x[%f] y[%f]\n", data->ray.x_step_vec.y, data->ray.y_step_vec.x);
}

void	save_quarter(t_data *data)
{
	double	a;

	a = data->ray.abs_ang;
	if (a > 0.0 && a < M_PI)
		data->ray.dir_vec.y = 1;
	else
		data->ray.dir_vec.y = -1;
	if (a > PI1_2 && a < PI3_2)
		data->ray.dir_vec.x = 1;
	else
		data->ray.dir_vec.x = -1;
	// data->ray.dir_vec.theta = data->ray.abs_ang;
	// calc_triangle_angle(&(data->ray.dir_vec));
	// data->ray.cur_pos.theta = data->ray.dir_vec.theta;
	// data->ray.cur_pos.triangle_angle = data->ray.dir_vec.triangle_angle;
}


void	get_line_height(t_data *data)
{
	int	wall_height;
	int	line_start;
	int	line_end;
	double	ray_len;

	//формула - гипотенуза на косинус прилежащего угла = катет
	//hypotenuse = ray_len
	//прилежащий угол это rel_ang (relative angle)
	//Косинус надо бы потом прекалькулейтиь

	ray_len = distance(&(data->ray.player_pos), &(data->ray.cur_pos));
	ray_len *= ft_abs(cos(data->ray.rel_ang));
	// printf("ray len = %f\n", ray_len);
	wall_height = ((double)WIN_HEIGHT / ray_len);
	line_start = (-wall_height / 2) + (WIN_HEIGHT / 2);
	if (line_start < 0)
		line_start = 0;
	line_end = (wall_height / 2) + (WIN_HEIGHT / 2);
	if (line_end >= WIN_HEIGHT)
		line_end = WIN_HEIGHT - 1;
	data->ray.wall_top = line_start;
	data->ray.wall_bot = line_end;
	
}


void	render_ray2d(t_data *data)
{
	// int	x = WIN_WIDTH / 2 - (data->map_w + data->player.x) * SCALE_2D;
	// int	y = WIN_HEIGHT/ 2 - (data->map_h + data->player.y) * SCALE_2D;
	int	x = (data->player.x) * SCALE_2D;
	int	y = (data->player.y) * SCALE_2D;
	
	int	x2 = (data->player.x + data->ray.x_step_vec.x) * SCALE_2D;
	int	y2 = (data->player.x + data->ray.x_step_vec.y) * SCALE_2D;
	draw_line(data, x, y, x2, y2, 0xFF28F11F);

	x2 = (data->player.x + data->ray.y_step_vec.x) * SCALE_2D;
	y2 = (data->player.x + data->ray.y_step_vec.y) * SCALE_2D;
	draw_line(data, x + 1, y + 1, x2 + 1, y2 + 1, 0xFFB3041F);
}

// void	showlines(t_data *data)
// {
// 	int	x = WIN_WIDTH / 2 - (data->map_w + data->player.x) * SCALE_2D;
// 	int	y = WIN_HEIGHT/ 2 - (data->map_h + data->player.y) * SCALE_2D;
// 	int	x2 = 
// 	int	
// 	int	ray_len = distance(&(data->ray.player_pos), &(data->ray.cur_pos)) * SCALE_2D;
// 	ray_len *= ft_abs(cos(data->ray.rel_ang));
// 	printf("(%d, %d) %d\n", x, y, ray_len);
// 	draw_line_c(data, x, y, ray_len, M_PI/2, 0xFF28F11F);
// }


void	calc_first_collisions(t_data *data)
{
	double aTan = -1/tan(data->ray.abs_ang);
	if (data->ray.abs_ang > M_PI && data->ray.abs_ang < M_PI * 2)
	{
		data->ray.cur_pos.y = (int)data->player.y;
		data->ray.cur_pos.x = data->player.y - data->ray.cur_pos.y * aTan + data->player.x;
	}
	else if (data->ray.abs_ang < M_PI && data->ray.abs_ang > 0)
	{
		data->ray.cur_pos.y = ceil(data->player.y);
		data->ray.cur_pos.x = data->player.y + data->ray.cur_pos.y * aTan + data->player.x;
	}
	else
	{
		data->ray.cur_pos.y = data->player.y;
		data->ray.cur_pos.x = data->player.x;
	}
	
}


void	cast_one_ray(t_data *data)
{
	save_quarter(data);
	calc_step_lengths(data);
	calc_first_collisions(data);
	//printf("after pre-step cur pos = %f, %f\n", data->ray.cur_pos.x, data->ray.cur_pos.y);
	// while (!is_solid(data, &(data->ray.cur_pos)))
	// {
	// 	ray_step(data);
	// }
	//printf("calculated ray end: (%f, %f)\n", data->ray.cur_pos.x, data->ray.cur_pos.y);
	get_line_height(data);
	render_ray2d(data);
	//showlines(data);
	//render_ray(data, data->ray.win_x, data->ray.wall_top, data->ray.wall_bot, 0x00);
}

double	get_trian_ang(double ang)
{
	while (ang > PI1_2)
		ang -= PI1_2;
	return (ang);
}

void	cast_rays(t_data *data)
{
	data->ray.win_x = 0;
	data->ray.abs_ang = data->player.theta - FOV_HALFANGLE;
	data->ray.rel_ang = -FOV_HALFANGLE;
	data->ray.tr_ang = get_trian_ang(data->ray.abs_ang);
	data->ray.player_pos.x = data->player.x;
	data->ray.player_pos.y = data->player.y;
	data->ray.player_pos.theta = data->player.theta;
	while (data->ray.win_x < WIN_WIDTH - 1)
	{
		cast_one_ray(data);
		data->ray.abs_ang += ANGLE_STEP;
		data->ray.rel_ang += ANGLE_STEP;
		data->ray.tr_ang += ANGLE_STEP;
		data->ray.tr_ang = get_trian_ang(data->ray.tr_ang);
		data->ray.win_x += 1;
	}
	//draw_line_c(data, data->player.x * SCALE_2D, data->player.y * SCALE_2D, 400, data->player.theta + M_PI, 0xFF1F1FF1);
	ft_printf("all rays cast\n");
}

void	drawscreen(void *ptr){
	t_data	*data = (t_data *)ptr;
	for(int i = 0; i < WIN_WIDTH; i++)
	{
		verLine(data, i, 0, WIN_HEIGHT - 1, 0x0);
	}
	draw_minimap(data);
	cast_rays(data);
	// printf("Rendered screen!\n");

	// draw_line(data, 400, 400, 800, 100, 0x11FFFF);
	// draw_line(data, 400, 400, 400, 800, 0x11FFFF);
	// draw_line(data, 800, 800, 800, 400, 0x11FFFF);
	// draw_line(data, 800, 800, 400, 800, 0x11FFFF);
	
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