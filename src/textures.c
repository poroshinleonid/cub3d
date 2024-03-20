#include "MLX42.h"
#include "config.h"
#include "libft.h"
#include "textures.h"
#include "funcs.h"
#include "unistd.h"
#include <math.h>

mlx_image_t	*load_one_texture(t_data *data, const char *path)
{
	const char		*err;
	mlx_texture_t	*texture;
	mlx_image_t		*res;

	texture = mlx_load_png(path);
	if (!texture)
	{
		err = mlx_strerror(mlx_errno);
		terminate(data, err);
	}
	res = mlx_texture_to_image(data->mlx, texture);
	if (!res)
	{
		err = mlx_strerror(mlx_errno);
		terminate(data, err);
	}
	return (res);
}

void	load_textures(t_data *data)
{
	data->textures.no = load_one_texture(data, data->textures.no_path);
	data->textures.so = load_one_texture(data, data->textures.so_path);
	data->textures.we = load_one_texture(data, data->textures.we_path);
	data->textures.ea = load_one_texture(data, data->textures.ea_path);
}

uint32_t	put_pixel_color(mlx_image_t *texture, double x, double y)
{
	uint8_t		*px;
	uint32_t	res;
	int			pos_x;
	int			pos_y;

	res = 0;
	pos_x = x * texture->width;
	pos_y = y * texture->height;
	px = (uint8_t *)(texture->pixels + (texture->width * pos_y + pos_x)
			* sizeof(uint32_t));
	res = px[3] | px[2] << 8 | px[1] << 16 | px[0] << 24;
	return (res);
}

mlx_image_t	*get_texture_type(bool north_dir, bool east_dir, t_point point,
		t_data *data)
{
	if ((int)point.x == point.x && (int)point.x % MAP_S == 0)
	{
		if (east_dir)
			return (data->textures.ea);
		return (data->textures.we);
	}
	if (north_dir)
		return (data->textures.no);
	return (data->textures.so);
}

uint32_t	get_texture_color(t_data *data, mlx_image_t *texture, t_point point,
		float text_y)
{
	float	cord;

	if (texture == data->textures.no || texture == data->textures.so)
		cord = point.x;
	else
		cord = point.y;
	while (cord > MAP_S)
		cord -= MAP_S;
	return (put_pixel_color(texture, cord / MAP_S, text_y));
}
