/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trusanov <trusanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:30:41 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 16:55:13 by trusanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "cub3d.h"
#include "funcs.h"
#include <math.h>

static void	normalize(float *dx, float *dy)
{
	float	scale;

	if (*dx == 0.0 && *dy == 0.0)
		return ;
	scale = sqrt((*dx * *dx) + (*dy * *dy));
	*dx /= scale;
	*dy /= scale;
	*dx *= PLAYER_SPEED;
	*dy *= PLAYER_SPEED;
}

static bool	is_wall(t_data *data, float x, float y)
{
	return (data->map.grid[(int)x / MAP_S][(int)y / MAP_S] == '1');
}

static void	nullify_if_wall(t_data *data, float *dx, float *dy)
{
	(void)data;
	(void)dx;
	(void)dy;
	if (is_wall(data, data->player.x + *dx * 3, data->player.y))
		*dx = 0.0;
	if (is_wall(data, data->player.x, data->player.y + *dy * 3))
		*dy = 0.0;
}

static void	move_dx_dy(t_data *data, float *dx, float *dy)
{
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		*dx += data->player.dx;
		*dy += data->player.dy;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		*dx -= data->player.dx;
		*dy -= data->player.dy;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		*dx -= data->player.dy;
		*dy += data->player.dx;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		*dx += data->player.dy;
		*dy -= data->player.dx;
	}
}

void	movement_hook(t_data *data)
{
	float	dx;
	float	dy;

	dx = 0.0;
	dy = 0.0;
	move_dx_dy(data, &dx, &dy);
	normalize(&dx, &dy);
	nullify_if_wall(data, &dx, &dy);
	data->player.x += dx;
	data->player.y += dy;
}
