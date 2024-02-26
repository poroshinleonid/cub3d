#include "textures.h"
#include "unistd.h"
#include "libft.h"


mlx_image_t* load_one_texture(t_data *data, const char *path)
{
    const char* err;
    mlx_texture_t *texture;
    mlx_image_t *res;

    texture = mlx_load_png(path);
    if (!texture) {
        err = mlx_strerror(mlx_errno);
        terminate(data, err);
    }
    res = mlx_texture_to_image(data->mlx_win, texture);
    if (!res) {
        err = mlx_strerror(mlx_errno);
        terminate(data, err);
    }
    return res;
}

void load_textures(t_data *data) {
    data->textures.no = load_one_texture(data, data->texture_names.no);
    data->textures.so = load_one_texture(data, data->texture_names.so);
    data->textures.we = load_one_texture(data, data->texture_names.we);
    data->textures.ea = load_one_texture(data, data->texture_names.ea);
}

uint32_t    put_pixel_color(mlx_image_t *texture, double x, double y)
{
    uint8_t*    px;
	uint32_t	res = 0;
    int pos_x;
    int pos_y;

    pos_x = x * texture->width;
    pos_y = y * texture->height;
    px = (uint8_t *)(texture->pixels + (texture->width * pos_y + pos_x) * sizeof(uint32_t));
	res = px[3] | px[2] << 8 | px[1] << 16 | px[0] << 24;
    return (res);
}
