#include "engine.h"

#include <math.h>

void rotation_hook(t_data *data) {
    if (mlx_is_key_down(data->mlx_win, MLX_KEY_A)) {
        data->player.theta += PL_ROT_SPEED;
        if (data->player.theta > 2 * PI)
            data->player.theta -= 2 * PI;
    }
    if (mlx_is_key_down(data->mlx_win, MLX_KEY_D)) {
        data->player.theta -= PL_ROT_SPEED;
        if (data->player.theta < 0.0)
            data->player.theta += 2 * PI;
    }
    data->player.dx = cos(data->player.theta) * PL_SPEED_SCALE;
    data->player.dy = sin(data->player.theta) * PL_SPEED_SCALE;
    data->tmp_d = data->cam_x;
    data->norm = sqrt(data->cam_x * data->cam_x + data->cam_x * data->cam_x);
    data->cam_x = data->cam_y / data->norm;
    data->cam_y = -data->tmp_d / data->norm;
}

void movement_hook(t_data *data) {
    if (mlx_is_key_down(data->mlx_win, MLX_KEY_UP)) {
        data->player.x += data->player.dx;
        data->player.y += data->player.dy;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_DOWN)) {
        data->player.x -= data->player.dx;
        data->player.y -= data->player.dy;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_RIGHT)) {
        data->player.x += data->player.dy;
        data->player.y -= data->player.dx;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_LEFT)) {
        data->player.x -= data->player.dy;
        data->player.y += data->player.dx;
    }
}

void ft_hook(void *param) {
    t_data *data;

    data = param;

    movement_hook(data);
    rotation_hook(data);
    printf("tae: %f %f %f\n", data->player.x, data->player.y, data->player.theta);
    cast_rays(data);
}
