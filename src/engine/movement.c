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
    data->player.dx = cos(data->player.theta);
    data->player.dy = sin(data->player.theta);
}

static void normalize(float* dx, float* dy) {
    float scale;

    if (*dx == 0.0 && *dy == 0.0)
        return;
    scale = sqrt((*dx * *dx) + (*dy * *dy));
    *dx /= scale;
    *dy /= scale;
    *dx *= PL_SPEED;
    *dy *= PL_SPEED;
}

void movement_hook(t_data *data) {
    float dx;
    float dy;
    
    dx = 0.0;
    dy = 0.0;
    if (mlx_is_key_down(data->mlx_win, MLX_KEY_W)) {
        dx += data->player.dx;
        dy += data->player.dy;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_S)) {
        dx -= data->player.dx;
        dy -= data->player.dy;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_D)) {
        dx += data->player.dy;
        dy -= data->player.dx;
    }
	if (mlx_is_key_down(data->mlx_win, MLX_KEY_A)) {
        dx -= data->player.dy;
        dy += data->player.dx;
    }
    normalize(&dx, &dy);
    data->player.x += dx;
    data->player.y += dy;
}

void ft_hook(void *param) {
    t_data *data;

    data = param;
    movement_hook(data);
    rotation_hook(data);
    printf("%f %f %f %f %f\n", data->player.x, data->player.y, data->player.theta, data->player.dx, data->player.dy);
}
