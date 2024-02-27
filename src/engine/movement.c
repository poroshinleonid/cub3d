#include "engine.h"

#include <math.h>

void rotation_hook(t_data *data) {
    if (mlx_is_key_down(data->mlx_win, MLX_KEY_LEFT)) {
        data->player.theta += PL_ROT_SPEED;
        if (data->player.theta > 2 * PI)
            data->player.theta -= 2 * PI;
    }
    if (mlx_is_key_down(data->mlx_win, MLX_KEY_RIGHT)) {
        data->player.theta -= PL_ROT_SPEED;
        if (data->player.theta < 0.0)
            data->player.theta += 2 * PI;
    }
    data->player.dx = cos(data->player.theta) * PL_SPEED_SCALE;
    data->player.dy = sin(data->player.theta) * PL_SPEED_SCALE;
}

void movement_hook(t_data *data) {
    if (mlx_is_key_down(data->mlx_win, MLX_KEY_W)) {
        data->player.x += data->player.dx;
        data->player.y += data->player.dy;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_S)) {
        data->player.x -= data->player.dx;
        data->player.y -= data->player.dy;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_D)) {
        data->player.x += data->player.dy;
        data->player.y -= data->player.dx;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_A)) {
        data->player.x -= data->player.dy;
        data->player.y += data->player.dx;
    }
}

void ft_hook(void *param) {
    t_data *data;

    data = param;
    drawscreen(data);
    movement_hook(data);
    rotation_hook(data);
    printf("%f %f %f\n", data->player.x, data->player.y, data->player.theta);
}
