#include "config.h"
#include "cub3d.h"
#include "funcs.h"
#include <math.h>

static t_point	first_vertical_point(t_data *data, float ra)
{
	t_point	p;

	p.exist = false;
	if (ra == PI / 2 || ra == 3 * PI / 2)
		return (p);
	if (ra < PI / 2 || ra > 3 * PI / 2)
		p.x = ceil_map(data->player.x);
	else
		p.x = floor_map(data->player.x);
	p.y = (p.x - data->player.x) * tan(ra) + data->player.y;
	if (p.y < 0 || p.x < 0 || p.y > data->map.h * MAP_S || p.x > data->map.w
		* MAP_S)
		return (p);
	p.exist = true;
	return (p);
}

static t_point	vertical_delta(float ra)
{
	t_point	delta;

	if (ra < PI / 2 || ra > 3 * PI / 2)
		delta.x = +MAP_S;
	else
		delta.x = -MAP_S;
	delta.y = tan(ra) * delta.x;
	return (delta);
}

static bool	is_vertical_in_obstacle(t_data *data, float x, float y)
{
	int	map_x;
	int	map_y;

	map_y = (int)y / MAP_S;
	map_x = (int)x / MAP_S;
	if (data->map.grid[map_x][map_y] == '1')
		return (true);
	map_x = (int)x / MAP_S - 1;
	if (map_x >= 0 && data->map.grid[map_x][map_y] == '1')
		return (true);
	return (false);
}

// first actual intersection with map
t_point	vertical_intersection(t_data *data, float ra)
{
	t_point	p;
	t_point	delta;

	while (ra > 2 * PI)
		ra -= 2 * PI;
	while (ra < 0)
		ra += 2 * PI;
	p = first_vertical_point(data, ra);
	if (p.exist == false)
		return (p);
	delta = vertical_delta(ra);
	while (p.y > 0 && p.x > 0 && p.y < data->map.h * MAP_S && p.x < data->map.w
		* MAP_S)
	{
		if (is_vertical_in_obstacle(data, p.x, p.y))
			return (p);
		p.x += delta.x;
		p.y += delta.y;
	}
	p.exist = false;
	return (p);
}
