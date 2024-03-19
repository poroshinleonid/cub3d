#include "MLX42.h"
#include "config.h"
#include "cub3d.h"
#include "funcs.h"
#include "textures.h"
#include <math.h>

static t_point	actual_intersection(t_data *data, float ra)
{
	t_point	horizontal;
	t_point	vertical;
	t_point	self;

	horizontal = horizontal_intersection(data, ra);
	vertical = vertical_intersection(data, ra);
	if (horizontal.exist == false)
		return (vertical);
	if (vertical.exist == false)
		return (horizontal);
	self.x = data->player.x;
	self.y = data->player.y;
	if (distance(self, horizontal) < distance(self, vertical))
		return (horizontal);
	return (vertical);
}

typedef struct s_ver_line
{
	int			x;
	int			start;
	int			end;
	uint32_t	color;
}				t_ver_line;

static void	draw_vertical_line(mlx_image_t *img, t_ver_line l)
{
	int	i;

	i = l.start;
	while (i < l.end)
	{
		mlx_put_pixel(img, l.x, i, l.color);
		i++;
	}
}

static void	draw_one_line(t_data *data, t_point point, int x, float ra)
{
	mlx_image_t		*texture;
	const t_point	self = {.x = data->player.x, .y = data->player.y,
		.exist = true};
	float			wall_h;
	float			fl_sky_h;
	float			y;

	wall_h = MAP_S / (distance(self, point) * cos(data->player.a - ra))
		* data->img_3d->height;
	fl_sky_h = (data->img_3d->height - wall_h) / 2;
	draw_vertical_line(data->img_3d, (t_ver_line){.x = x, .start = 0,
		.end = fl_sky_h, .color = SKY_CLR});
	draw_vertical_line(data->img_3d, (t_ver_line){.x = x,
		.start = fminf(fl_sky_h + wall_h, data->img_3d->height),
		.end = data->img_3d->height, .color = FLOOR_CLR});
	texture = get_texture_type(self.y - point.y > 0, self.x - point.x < 0,
			point, data);
	y = fl_sky_h;
	while (y < fl_sky_h + wall_h)
	{
		if ((uint32_t)y < data->img_3d->height)
			mlx_put_pixel(data->img_3d, x, y, get_texture_color(data, texture,
					point, (y - fl_sky_h) / wall_h));
		y += 1;
	}
}

static void	draw_one_ray(t_data *data, float ra, int x)
{
	t_point	intersection;

	intersection = actual_intersection(data, ra);
	if (intersection.exist)
		draw_one_line(data, intersection, x, ra);
}

void	draw_rays(t_data *data)
{
	int		x;
	float	min_ra;
	float	ra;

	min_ra = data->player.a - FOV / 2;
	x = 0;
	while (x < data->img_width)
	{
		ra = min_ra + FOV * x / data->img_width;
		draw_one_ray(data, ra, x);
		x++;
	}
}
